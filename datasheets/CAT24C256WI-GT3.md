---
part_number: CAT24C256WI-GT3
manufacturer: onsemi
component_type: 256-Kb I2C CMOS serial EEPROM
package: SOIC-8, JEDEC (W suffix, case 751BD)
datasheet_revision: 16
datasheet_date: 2022-02
source_doc: CAT24C256/D, Rev. 16
source_url: https://www.onsemi.com/pdf/datasheet/cat24c256-d.pdf
board_ref: drive.md DSP.SchDoc EEPROM nets
---

# CAT24C256WI-GT3 — 256-Kb I2C Serial EEPROM

Firmware-relevant extract from onsemi CAT24C256/D Rev. 16. Mechanical drawings and packing details are omitted.

Board part: **CAT24C256WI-GT3** (marking `24256E`, industrial −40 °C to +85 °C, NiPdAu, tape & reel).

## Quick Reference

| Parameter | Value |
|---|---|
| Organization | 32,768 × 8 bits (32 KB) |
| Pages | 512 pages × 64 bytes |
| Address word | 16-bit; A15 is don’t-care; A14..A6 = page, A5..A0 = byte in page |
| Valid firmware address | `0x0000` .. `0x7FFF` |
| I2C | Slave; Standard 100 kHz, Fast 400 kHz, Fast-Plus 1 MHz |
| Slave address | `1010 A2 A1 A0 R/W` |
| Write cycle tWR | 5 ms max (all I2C speeds) |
| Page write buffer | 64 bytes; extra bytes wrap **inside the selected page** |
| Hardware WP | Entire memory; WP high rejects writes |
| Delivery state | All bytes `0xFF` |
| VCC | 1.8 V to 5.5 V |
| Endurance | 1,000,000 program/erase cycles |
| Data retention | 100 years |
| ECC | 6 ECC bits correct 1 bit error in 4 data bytes. onsemi recommends writes in multiples of 4 bytes for max endurance. Not enforced in firmware yet. |

This board runs I2C-A at Fast 400 kHz (see `i2c_interface.h`).

## This Board (drive.md)

| CAT24C256 pin | Net | TMS320F28335 | Notes |
|---|---|---|---|
| SDA | `EEPROM_SDA` | pin 74 / GPIO32 / SDAA | 5.1 k pull-up to +3V3 |
| SCL | `EEPROM_SCL` | pin 75 / GPIO33 / SCLA | 5.1 k pull-up to +3V3 |
| A1 | `EEPROM_A1` | pin 66 / GPIO22 | DSP GPIO; init drives low |
| WP | `EEPROM_WP` | pin 67 / GPIO23 | DSP GPIO; init drives low (writes allowed) |
| A0, A2 | — | not DSP GPIOs | Assumed GND → 7-bit slave `0x50` when A1 = 0 |
| VCC | +3V3 | — | |
| VSS | GND | — | |

7-bit slave with A2 = A0 = 0:

```text
slave = 0x50 | (A1 << 1)     /* A1=0 → 0x50, A1=1 → 0x52 */
```

## SOIC-8 Pinout (top view)

```text
A0  [1]        [8] VCC
A1  [2]        [7] WP
A2  [3]        [6] SCL
VSS [4]        [5] SDA
```

| Pin | Name | Function |
|---:|---|---|
| 1 | A0 | Device address LSB; internal pull-down |
| 2 | A1 | Device address; internal pull-down |
| 3 | A2 | Device address MSB; internal pull-down |
| 4 | VSS | Ground |
| 5 | SDA | Serial data, open-drain |
| 6 | SCL | Serial clock input |
| 7 | WP | Write protect; internal pull-down. HIGH = all writes inhibited |
| 8 | VCC | Supply |

WP / A0 / A1 / A2 have a strong internal pull-down when the pin is below ~0.5 × VCC. An external driver must source that current to take the pin HIGH.

## I2C Protocol

CAT24C256 is always a slave. START = SDA falling while SCL high. STOP = SDA rising while SCL high. SDA must be stable while SCL is high.

Slave address byte:

```text
1 0 1 0 | A2 A1 A0 | R/W
```

R/W = 0 write, 1 read. Slave ACKs by pulling SDA low on the 9th clock.

STOP after a write starts the internal write cycle. STOP after a read returns the device to standby.

### Byte write

START → slave (W) → address high (A15..A8, A15 don’t-care) → address low (A7..A0) → 1 data byte → STOP.

All four bytes are ACK’d. STOP starts tWR. During tWR the slave **does not ACK** any Read or Write (NACK on slave address).

### Page write

Same as byte write, then up to 64 data bytes before STOP. Address high A14..A6 selects the page; A5..A0 is the start byte in that page.

The on-chip address counter increments after each data byte **within the page only**. If the master sends more than 64 data bytes, later bytes overwrite earlier bytes in that page (page wrap). Crossing into the next 64-byte page in one STOP does **not** write the next page.

tWR starts at STOP, same 5 ms max as a byte write.

### Acknowledge polling

While tWR is in progress, a Selective Read (or any slave-address probe) gets a NACK on the slave address. When tWR finishes, the slave ACKs again. The F28335 I2C driver retries NACK for this busy window. Do not also busy-wait 5 ms in the EEPROM layer.

### Hardware write protect

WP HIGH protects the **entire** memory. Floating or grounded WP does not affect writes.

WP is strobed on the last falling SCL edge immediately before the first data byte (`tSU:WP` min 0, Fast `tHD:WP` min 2.5 µs).

If WP is HIGH at that strobe:

- Address bytes still ACK
- First **data** byte is **NACK’d**
- Write is discarded

That data-byte NACK is not tWR busy (busy NACKs the **slave address**). Firmware must not queue a write while WP is already high, or the I2C NACK-retry path will treat WP as tWR.

### Immediate address read

START → slave (R) → data bytes… → NoACK → STOP.

Starts at the internal address counter (byte after the last access; wraps `0x7FFF` → `0x0000`).

### Selective read (used by this BSP)

START → slave (W) → address high → address low → repeated START → slave (R) → data… → NoACK → STOP.

This is a dummy write of the 16-bit address, then a read. Sequential read: master ACKs each data byte except the last. Sequential read **wraps the whole 32 KB**, not the 64-byte page.

## Firmware limits on this BSP

I2C-A TX FIFO is 16 bytes. Two of those are the address word, so one queued access is **1..14 data bytes** (`I2C_MAX_BYTES`).

| Check | Write | Read |
|---|---|---|
| `n` in 1..14 | required | required |
| `addr` and `addr + n - 1` in `0x0000..0x7FFF` | required | required |
| Range stays in one 64-byte page | required (else page wrap) | not required |
| WP high | reject | allowed |

Page test: `(addr & ~63) == ((addr + n - 1) & ~63)`.

## Timing used by firmware (Fast 400 kHz, VCC 1.8–5.5 V)

| Symbol | Parameter | Fast min / max |
|---|---|---|
| FSCL | Clock | 400 kHz max |
| tWR | Internal write after STOP | 5 ms max |
| tPU | Power-up to ready | 1 ms max |
| tHD:WP | WP hold after data-byte SCL falling | 2.5 µs min |

POR: device enters standby after VCC crosses the POR rising threshold and resets if VCC drops below the falling threshold (brown-out protect).
