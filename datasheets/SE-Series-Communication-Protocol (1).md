# SE-Series Absolute Encoder — Communication & Protocol Reference

> Source: SE.pdf datasheet (Jilin Sansheng Sensing Technology Co., Ltd.)
> Scope: This file extracts **only** communication-interface and protocol content (baud rates, frame formats, register maps, timing diagrams, wiring, ordering codes). Mechanical dimensions, mounting drawings, and application blurbs from the source PDF are intentionally omitted. All text has been translated to English.

---

## 1. Interfaces & Protocols Supported

| Interface | Protocols carried |
|---|---|
| RS485 | Free protocol (proprietary), Modbus-RTU |
| RS422 | SSI, BISS-C |

Common electrical spec across the SE-series: DC5V supply, 50mA (Max) current draw. Position code: Binary or Gray code.

---

## 2. RS485 — Free Protocol / Modbus-RTU

### 2.1 Link parameters
- **Communication mode**: periodic/scheduled transmission
- **Baud rate**: 2 MB/s, 921.6 KB/s, 115.2 KB/s, 57.6 KB/s, 9.6 KB/s
- **Update rate**: 10 KHz, 4 KHz, 1 KHz, 0.5 KHz, 0.1 KHz
- **Serial framing**: 1 start bit, 8 data bits, 1 stop bit, no parity

### 2.2 Free Protocol (RS485) — 8-byte request/response frame

```
Byte1     Byte2        Byte3-4                Byte5-6            Byte7-8
Address   Func. code   Register start addr    Register count     CRC (low byte first)
```

**Example — read angle data** (station 01):
```
01 03 00 00 00 02 C4 0B
```

**Example — master sets encoder baud rate** (station 01, baud → 115200):
```
01 06 00 02 00 01 25 CA
```

**Slave response — set successful** (echoes the write frame):
```
01 06 00 02 00 01 25 CA
```

**Slave response — position data**:
```
Byte1     Byte2        Byte3          Byte4-7        Byte8
Address   Func. code   Data length    Angle data     CRC
```
Example:
```
01 03 04 00 01 2D B6 F5
```

### 2.3 Modbus-RTU function/register table

| # | Function | Request bytes (hex) | Note |
|---|---|---|---|
| 1 | Read angle data | `01 03 00 00 00 02 C4 0B` | |
| 2 | Read station address | `AD 03 00 01 00 01 CD A6` | |
| 3 | Read baud rate | `01 03 00 02 00 01 25 CA` | See note ① |
| 4 | Read rotation direction | `01 03 00 03 00 01 74 0A` | 1 = clockwise (CW), 0 = counter-clockwise (CCW) |
| 5 | Set station address | `01 06 00 01 00 01 19 CA` | |
| 6 | Set baud rate | `01 06 00 02 00 01 E9 CA` | See note ① |
| 7 | Set rotation direction | `01 06 00 03 00 01 74 0A` | 1 = CW, 0 = CCW |
| 8 | Set zero position | `01 06 00 05 00 01 58 0B` | |

Note ①: baud rate options for this Modbus-RTU function set are 115.2 KB/s, 57.6 KB/s, 19.2 KB/s, 9.6 KB/s.

### 2.4 Streaming binary output frame (periodic angle-only output)

Used for the periodic automatic angle broadcast, distinct from the register read/write frames above.

**24-bit-and-below data format (6 bytes):**

| Byte1 | Byte2 | Byte3 | Byte4 | Byte5 | Byte6 |
|---|---|---|---|---|---|
| 0xFF (frame header) | 0x81 (frame header) | Angle data, high byte | Angle data, mid byte | Angle data, low byte | Checksum (of header + angle bytes, low byte kept) |

**16-bit-and-below data format (5 bytes):**

| Byte1 | Byte2 | Byte3 | Byte4 | Byte5 |
|---|---|---|---|---|
| 0xFF (frame header) | 0x81 (frame header) | Angle data, high byte | Angle data, low byte | Checksum (low byte kept) |

**Angle conversion** (16-bit-and-below example, per datasheet):
```
angle = (Byte3 × 256 + Byte4) / 2^n      where n = encoder resolution in bits
```
For reliability, the receiver should validate both the frame header (0xFF 0x81) and the checksum byte before accepting the data.

> ⚠️ Source-fidelity note: the original table layout for this frame (footnote markers 1–4) was partially garbled by PDF extraction. The reconstruction above follows the datasheet's own angle-conversion formula (`(Byte3*256+Byte4)/2^n`), which unambiguously confirms Byte3 = high byte and Byte4 = low byte for the 16-bit case.

---

## 3. RS422 — SSI Protocol

Timing diagram signals: `CLK+` (clock) and `DATA+` (data), MSB first, LSB last, then idle (Tf) before the next frame.

| Parameter | Min | Max | Unit |
|---|---|---|---|
| Ts (setup) | 50 | — | ns |
| Tc (clock period) | 100 | — | ns |
| To (data valid) | 0.075 (Typ: 20) | 24 | ns |
| T1 | 25 | To | ns |
| Th | 25 | To | ns |
| Tf (idle before next frame) | — | To | — |

---

## 4. RS422 — BISS-C Protocol

Frame sequence on `DATA+`: `Ack → Start → 0 → Position → Error → Warn → CRC (6-bit)`, clocked by `CLK+`, followed by idle period `Tf`.

| Parameter | Min | Max | Unit |
|---|---|---|---|
| Tb | 50 | 5×Tc | ns |
| Tc | — | — | ns |
| To | 0.075 (Typ: 20) | 24 | µs |
| T1 | 20 | To | ns |
| Th | 20 | To | ns |
| Tf (idle before next frame) | — | To | µs |

Field notes:
- **POSITION**: encoder resolution bits (angle data).
- **ERROR**: active low; flags data/temperature faults.
- **WARN**: active low; indicates a warning/error condition is present.
- **CRC (6-bit)**: polynomial `X^6 + X^1 + X^0`; most-significant bit first, and the result is inverted; the start bit and the leading `0` are excluded from the CRC calculation.

---

## 5. Interface Wiring / Wire Colors

**RS485 interface** (Free protocol / MODBUS / NRZ):

| Red | Black | Yellow | Green | White |
|---|---|---|---|---|
| VCC | 0V (GND) | TXD+ | TXD− | CLR/NC |

**RS422 interface** (SSI / BISS-C):

| Red | Black | Yellow | Green | White |
|---|---|---|---|---|
| VCC | 0V (GND) | TXD+ | TXD− | RXD+ / RXD− |

---

## 6. Ordering Code — Protocol-Related Fields

Full part number pattern: `SE [OD] [bore] [mounting type] [bit resolution] [voltage] [interface] [baud rate/update rate] [code type] [cable exit] [customer code]`

Fields relevant to communication:

**Voltage**
| Code | Meaning |
|---|---|
| 1 | DC5V |
| 2 | DC24V |

**Interface / Protocol**
| Code | Meaning |
|---|---|
| T | RS485 |
| S | SSI |
| B | BISS-C |
| M | Modbus |

**Baud rate / Update rate** (applies under RS485-T)
| Code | Baud rate / Update rate |
|---|---|
| A | 2 MB/s – 10 KHz |
| B | 921.6 KB/s – 4 KHz |
| C | 115.2 KB/s – 1 KHz |
| D | 57.6 KB/s – 0.5 KHz |
| E | 9.6 KB/s – 0.1 KHz |

**Code type**
| Code | Meaning |
|---|---|
| B | Binary |
| G | Gray code |

**Cable exit**
| Code | Meaning |
|---|---|
| T | Rear exit |
| G | Side exit |

---

## 7. Resolution / Accuracy Reference (needed to decode angle data)

Use with the angle-conversion formula in §2.4 (`angle = raw / 2^n`).

| Bits (n) | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 |
|---|---|---|---|---|---|---|---|---|---|---|
| Resolution | 40″ | 20″ | 10″ | 5″ | 2.5″ | 1.3″ | 0.6″ | 0.3″ | 0.15″ | 0.08″ |
| Accuracy ≤± | 80″ | 40″ | 20″ | 15″ | 7.5″ | 5″ | 3″ | 2.5″ | 2″ | 2″ |

**Resolution by model (from datasheet spec pages):**

| Model | Resolution (n, Max) |
|---|---|
| SE38S | 18 bit |
| SE38T | 22 bit |
| SE50T | 18 bit |
| SE58S | 18 bit |
| SE58T | 18 bit |
| SE80T | 24 bit |
| SE100T | 24 bit |
| SE110T | 24 bit |
| SE180 | 24 bit |

---

## 8. Contact (from datasheet footer)
- Jilin Sansheng Sensing Technology Co., Ltd.
- Website: www.san-sheng.net
- Email: gao@san-sheng.net
- Phone: 400-0431-806
- Document version: 20220718-A
