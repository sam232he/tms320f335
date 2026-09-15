# SCHA63T-K01 — 6-DOF XYZ Gyroscope + XYZ Accelerometer (SPI)

> Murata Doc. No. 8157, Rev. 2 (rev2_1640-21648-0031-E-0222)
> Source PDF: `SCHA63T-K01rev2_1640-21648-0031-E-0222.pdf`
> This file is a structured, agent/LLM-friendly reference distilled from the full 65-page
> datasheet. All register maps, SPI frame formats, timing figures and formulas needed to
> write a driver are preserved. Narrative/marketing content is condensed.

---

## 1. Quick Facts

| Item | Value |
|---|---|
| Type | Combined 3-axis angular rate (gyro) + 3-axis accelerometer, digital SPI |
| Gyro range | ±125 °/s |
| Accel range | ±6 g |
| Operating temp | −40 °C … +110 °C |
| Supply | 3.0 V … 3.6 V (V3p3A, V3p3D), typ 3.3 V |
| Interfaces | 2 independent physical SPI buses: **UNO** ASIC (X-gyro + XYZ-accel) and **DUE** ASIC (Y,Z-gyro) |
| Package | SOIC-32, 19.71 × 12.15 × 4.6 mm |
| Output data rate | F_prim / 2 (≈8–10 kHz depending on channel/ASIC) |
| Safety | Can be used in Safety Critical Applications; extensive self-diagnostics |
| Military/export restriction | See https://www.murata.com/en-global/support/militaryrestriction |

### Order codes
| Order Code | Description | Packing | Qty |
|---|---|---|---|
| SCHA63T-K01-004 | Gyro ±125dps, Accel ±6g | Bulk | 4 pcs |
| SCHA63T-K01-05 | Gyro ±125dps, Accel ±6g | Tape & Reel | 50 pcs/Reel |
| SCHA63T-K01-6 | Gyro ±125dps, Accel ±6g | Tape & Reel | 600 pcs/Reel |

### Abbreviations
ASIC=Application Specific IC · CCM=Channel calibration & monitoring · Cpk=Process Capability Index ·
CSB=Chip Select · CST=Continuous Self Test · DPS=Degrees per second · DUE=ASIC for Z,Y-axis rate ·
FFB=Force Feedback (gyro principle) · FS=Full scale · HPC=High Performance Combo · MOSI/MISO=SPI data lines ·
MCU=Microcontroller · RT=Room Temperature · SCK=Serial Clock · SPI=Serial Peripheral Interface ·
UNO=ASIC for X-axis rate + XYZ-axis accelerometer · F_prim=Gyro primary frequency ·
Rx/Ry/Rz=Rate X/Y/Z · Ax/Ay/Az=Accel X/Y/Z

---

## 2. Two-ASIC Architecture (critical for driver design)

The component contains **two separate ASICs, each with its own physical SPI bus** (CSB, SCK, MOSI, MISO):

- **UNO** — Rate X (Rx) + Accelerometer X, Y, Z + Temperature
- **DUE** — Rate Y (Ry) + Rate Z (Rz) + Temperature

Both ASICs must be initialized/reset/read independently. The same SCK line is recommended to drive both ASICs when communicating with them in parallel.

| Output | ASIC | Register (hex) |
|---|---|---|
| Rate X | UNO | 01h |
| Rate Y | DUE | 03h |
| Rate Z | DUE | 01h |
| Accel X | UNO | 04h |
| Accel Y | UNO | 05h |
| Accel Z | UNO | 06h |
| Temperature | both | 07h |

### Pinout summary (32-pin SOIC)

| Pin# | Name | ASIC | Type | Description |
|---|---|---|---|---|
| 1 | HEAT/EMC_GND | – | GND | Heatsink interface to GNDA |
| 2 | MOSI | DUE | DIN | SPI data in |
| 3 | CSB | DUE | DIN | SPI chip select |
| 4 | SCK | DUE | DIN | SPI clock |
| 5 | MISO | DUE | DOUT | SPI data out |
| 6 | V3p3D | DUE | SUPPLY | Digital supply |
| 7 | V3p3A | DUE | SUPPLY | Analog supply |
| 8 | GNDD | DUE | GND | Digital ground (connect ext. to GNDA) |
| 9 | VDDD | DUE | AOUT | Regulated digital core supply (ext. cap, see §9.1) |
| 10 | GNDA/EMC_GND | UNO | GND | Analog ground (connect ext. to GNDD) |
| 11 | VDDA | UNO | AOUT | Regulated analog core supply (ext. cap) |
| 12 | RESERVED | UNO | – | Factory use only — connect to GND |
| 13 | TIOP | UNO | – | Factory use only — connect to GND |
| 14 | TI/EXTRESN | UNO | DIN | Optional ext. reset, 3.3V logic-compatible Schmitt-trigger, internal pull-up, LOW→HIGH restarts system, min low time 100 µs |
| 15 | TION | UNO | – | Factory use only — connect to GND |
| 16 | HEAT/EMC_GND | – | GND | Heatsink interface to GNDA |
| 17 | HEAT/EMC_GND | – | GND | Heatsink interface to GNDA |
| 18 | MOSI | UNO | DIN | SPI data in |
| 19 | CSB | UNO | DIN | SPI chip select |
| 20 | SCK | UNO | DIN | SPI clock |
| 21 | MISO | UNO | DOUT | SPI data out |
| 22 | V3p3D | UNO | SUPPLY | Digital supply |
| 23 | V3p3A | UNO | SUPPLY | Analog supply |
| 24 | GNDD | UNO | GND | Digital ground |
| 25 | VDDD | UNO | AOUT | Regulated digital core supply |
| 26 | GNDA/EMC_GND | DUE | GND | Analog ground |
| 27 | VDDA | DUE | AOUT | Regulated analog core supply |
| 28 | RESERVED | DUE | – | Factory use only — connect to GND |
| 29 | TIOP | DUE | – | Factory use only — connect to GND |
| 30 | TI/EXTRESN | DUE | DIN | Optional ext. reset (same as pin 14) |
| 31 | TION | DUE | – | Factory use only — connect to GND |
| 32 | HEAT/EMC_GND | – | GND | EMC protection and ground |

### Measurement axes / accelerometer output convention
Right-handed XYZ, pin 1 marks the reference corner. Sensitivity = 4905 LSB/g typ. Static 1g output per face orientation (Table 11):

| Orientation (gravity vector into that face) | Ax | Ay | Az |
|---|---|---|---|
| Flat, top side up | 0 g (0 LSB) | 0 g (0 LSB) | +1 g (+4905 LSB) |
| Flat, bottom side up (flipped) | 0 g (0 LSB) | 0 g (0 LSB) | −1 g (−4905 LSB) |
| On edge, long side A | 0 g (0 LSB) | +1 g (+4905 LSB) | 0 g (0 LSB) |
| On edge, long side B (opposite) | 0 g (0 LSB) | −1 g (−4905 LSB) | 0 g (0 LSB) |
| On edge, short side (pin-1 side) | −1 g (−4905 LSB) | 0 g (0 LSB) | 0 g (0 LSB) |
| On edge, short side (opposite pin-1 side) | +1 g (+4905 LSB) | 0 g (0 LSB) | 0 g (0 LSB) |

> ⚠️ This table gives the correct magnitude/sign pattern from Table 11, but which physical edge/pin-1 corner maps to which row is a package-orientation detail best confirmed visually against Figure 3 / Table 11 in the source PDF before wiring a mechanical mount.

---

## 3. Electrical Specifications

### 3.1 General (Table 1)
| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| Supply voltage V3p3A | | 3.0 | 3.3 | 3.6 | V |
| Supply voltage V3p3D | | 3.0 | 3.3 | 3.6 | V |
| Supply current V3p3A (DUE) | | | 8.5 | | mA |
| Supply current V3p3D (DUE) | | | 9.5 | | mA |
| Supply current V3p3A+D (DUE) | | | 18 | | mA |
| Supply current V3p3A (UNO) | | | 6.25 | | mA |
| Supply current V3p3D (UNO) | | | 6.25 | | mA |
| Supply current V3p3A+D (UNO) | | | 12.5 | | mA |
| Total current I_TOTAL (UNO+DUE) | −40…+110 °C, CC | | 30.5 | | mA |
| Total current during reset (UNO+DUE) | | | | 4 | mA |
| Output update rate | Gyro/Accel/Temp | | F_prim/2 | | Hz |
| **TMODE** | Wait after supply in-spec/reset before SPI functional | 25 | | | ms |
| **TSPIR** | SPI comm. not allowed for 2 ms after SPI Hardreset | 2 | | | ms |

### 3.2 Gyroscope performance (Table 2, VDD=3.3V, RT unless noted)
| Parameter | Condition | Axis | Min | Typ | Max | Unit |
|---|---|---|---|---|---|---|
| Measurement range | min saturation flag | XYZ | ±125 | | | °/s |
| Offset (after cal, 3σ) | N>234 | XYZ | −0.8 | 0 | 0.8 | °/s |
| Offset temp dependency | −40…110°C, 3σ | XY | −0.65 | | 0.65 | °/s |
| | | Z | −0.085 | | 0.085 | °/s |
| Offset change/lifetime (1000h HTOL @125°C) | | XY | −0.22 | | 0.22 | °/s |
| | | Z | −0.06 | | 0.06 | °/s |
| Sensitivity (after cal @125°/s, 3σ) | N=30 | XYZ | 156.8 | 160 | 163.2 | LSB/°/s |
| Sensitivity temp dependency | −40…110°C | XY | −0.2 | | 0.6 | % |
| | | Z | 0 | | 0.2 | % |
| Sensitivity change/lifetime (1000h HTOL) | | XY | −0.75 | | 0.75 | % |
| | | Z | −0.21 | | 0.21 | % |
| Linearity error | end-point fit ±125°/s | XYZ | | 0.035 | 0.065 | °/s |
| Noise density | 3σ, N=90 | XYZ | | 0.0011 | 0.0017 | °/s/√Hz |
| Angle random walk | 3σ, N=90 | XYZ | | 0.07 | 0.10 | °/√h |
| Bias instability | RT, Allan var min/0.664 | XYZ | | 1.11 | 1.89 | °/h |
| Orthogonality error | after ext. cross-axis comp. | XYZ | −0.25 | | 0.25 | % |
| Amplitude response −3dB | 13 Hz filter | XYZ | 12.1 | 13.6 | 14.3 | Hz |
| | 20 Hz filter | | 18.5 | 20.2 | 21.5 | Hz |
| | 46 Hz filter | | 42.1 | 46.1 | 48.9 | Hz |
| | 300 Hz filter | | 280 | 305.8 | 325.6 | Hz |
| Power-on start-up time (after SPI power-on cmd) | 13/20 Hz filter, MAX | XYZ | | | 620 | ms |
| | 46/300 Hz filter, MAX | | | | 500 | ms |
| F_prim | sensor element clock | XY | 15.8 | 16.8 | 17.8 | kHz |
| | | Z | 18.3 | 19.3 | 20.3 | kHz |
| Output update rate | | XYZ | | F_prim/2 | | |
| G sensitivity (1g static) | 3σ, N=48 | XYZ | | | 0.006 | (°/s)/g |

Notes: min/max = validation mean ±3σ, not guaranteed; specification valid 24 h after reflow; system-level evaluation required.

### 3.3 Accelerometer performance (Table 3, VDD=3.3V, RT unless noted)
| Parameter | Condition | Axis | Min | Typ | Max | Unit |
|---|---|---|---|---|---|---|
| Measurement range | min saturation flag | XYZ | 6 | | | g |
| Offset (after cal, 3σ) | N=234 | XYZ | −13.5 | | 13.5 | mg |
| Offset temp dependency | −40…110°C | XYZ | −7.3 | | 7.3 | mg |
| Offset change/lifetime (1000h HTOL) | | XYZ | −22 | | 22 | mg |
| Sensitivity (after cal @±1g, 3σ) | N=234 | XYZ | 4899 | 4905 | 4911 | LSB/g |
| Sensitivity temp dependency | −40…110°C | XYZ | −0.15 | | 0.15 | % |
| Sensitivity change/lifetime | | XYZ | −0.06 | | 0.06 | % |
| Linearity error | end-point fit ±6g | XYZ | 1.9 | 6.3 | 17.2 | mg |
| | end-point fit ±1g | XYZ | | | 1 | mg |
| Noise density | 3σ, N=12 | XYZ | | 59.5 | 66.0 | µg/√Hz |
| Velocity random walk | 3σ, N=12 | XYZ | | 35.0 | 38.8 | mm/s/√h |
| Bias instability | RT, Allan var min/0.664 | XYZ | | 12.2 | 18.3 | µg |
| Orthogonality error | after ext. cross-axis comp. | XYZ | −0.14 | | 0.14 | % |
| Amplitude response −3dB | 13 Hz filter | XYZ | 13.2 | 13.7 | 14.2 | Hz |
| | 20 Hz filter | | 19.7 | 20.3 | 20.8 | Hz |
| | 46 Hz filter | | 45.0 | 46.3 | 47.5 | Hz |
| | 300 Hz filter | | 247.4 | 264.2 | 286.7 | Hz |
| Power-on start-up (after SPI cmd) | 13/20 Hz, MAX | XYZ | | | 450 | ms |
| | 46/300 Hz, MAX | | | | 320 | ms |
| Output update rate | tied to X-gyro F_prim/2 | XYZ | 7.9 | 8.4 | 8.9 | kHz |

### 3.4 Temperature sensor (Table 4)
| Parameter | Min | Typ | Max | Unit |
|---|---|---|---|---|
| Temperature signal range | −50 | | +150 | °C |
| Temperature signal sensitivity | | 30 | | LSB/°C |

**Conversion:** `Temperature[°C] = 25 + (TEMP / 30)` where TEMP = signed (2's complement) decimal register value.

### 3.5 Absolute maximum ratings (Table 6)
| Parameter | Remark | Min | Max | Unit |
|---|---|---|---|---|
| VDD | Supply voltage | −0.3 | 4.3 | V |
| AIN/AOUT | analog I/O pins | −0.3 | VDD+0.3 (4.3) | V |
| DIN/DOUT | digital I/O pins | −0.3 | VDD+0.3 (4.3) | V |
| Topr | operating temp | −40 | 110 | °C |
| Tstg | storage temp | −40 | 150 | °C |
| ESD_HBM | Q100-002 | ±2000 | | V |
| ESD_MM | Q100-003 | ±200 | | V |
| ESD_CDM | Q100-011 | ±500 (±750 corner pins) | | V |
| US | Ultrasonic agitation (cleaning/welding) | Prohibited | | |

### 3.6 SPI DC characteristics (Table 8, VDD=3.3V unless noted)
**SCLK:** VinHigh 2…V3p3D+0.3 V, VinLow −0.3…0.8 V, Vhy 0.2 V, Isource(pull-down) 24–36 µA, Cin ≤6 pF.
**CSB (pull-up, active low):** VinHigh 2…V3p3D+0.3 V, VinLow −0.3…0.8 V, Isource(pull-up,Vin=0V) 24–36 µA, Vin_open ≥2 V.
**MOSI (pull-down):** VinHigh 2…V3p3D+0.3 V, VinLow −0.3…0.8 V, Isource 24–36 µA, Vin_open ≤0.3 V.
**MISO (tri-state):** VoutHigh(−1mA) ≥V3p3D−0.5 V; VoutLow(+1mA) ≤0.5 V; Iout_Hz −1…1 µA; Cload ≤200 pF.

**EXTRESN pin (Table 9):** VinHigh 2…V3p3A+0.3 V, VinLow −0.3…0.8 V; Isource start-up inactive 60–160 µA, active 30–80 µA.

### 3.7 SPI AC characteristics (Table 10) — SPI Mode 0 (CPOL=0, CPHA=0)
| Symbol | Description | Min | Nom | Max | Unit |
|---|---|---|---|---|---|
| fSPI | SCLK frequency | 0.1 | – | 10 | MHz |
| tSPI | SCLK period | – | 1/fSPI | – | |
| thigh | SCLK high time | 35 | tSPI/2 | – | ns |
| tlow | SCLK low time | 35 | tSPI/2 | – | ns |
| tsucs | CSB setup (falling edge to SCLK rising) | 40 | tSPI/2 | – | ns |
| tsusi | MOSI setup before SCLK rising | 10 | – | – | ns |
| thsi | MOSI hold after SCLK rising | 20 | – | – | ns |
| thcs | CSB hold (SCLK falling to CSB rising) | 30 | tSPI/2 | – | ns |
| thics | min CSB high between transfers | 30 | tSPI/2 | | ns |
| trise/fall_in | SCK/MOSI rise/fall (master) | – | – | 0.15×tSPI | ns |
| td1 | CSB falling → MISO valid (slave) | – | – | 30 | ns |
| td2 | SCLK falling → MISO valid (slave) | 0 | – | 30 | ns |
| ttri | CSB rising → MISO tri-state | – | – | 25 | ns |
| trise/fall_out | MISO rise/fall (slave, slew-rate selectable via Mode reg 19h) | 4 | 10 | 16 | ns |

Data captured on SCK rising edge (MOSI), propagated on SCK falling edge (MISO). CSB falling edge starts, CSB rising edge terminates transmission.

---

## 4. Package / Footprint

- Package: SOIC-32, dimensions 19.71 × 12.15 (±0.25) × 4.60 (±0.20) mm, lead pitch 0.60 mm (32×), standoff 0.40±0.15 mm.
- Total weight < 1300 mg. RoHS/ELV compliant, lead-free SMD, EOL sensitive — protection required.
- Recommended PWB pad layout: 15 mm × 13 mm outer footprint envelope, pad pitch 1.0 mm, pad width 0.7 mm, inner pad spacing 10.8 × 8.6 mm.
- Ultrasonic agitation (cleaning, welding) is **prohibited**.
- Specification valid 24 hours after reflow.

---

## 5. Power-Up / Reset / Start-up Sequence

> Note: Figure 7 in the source is a flowchart image; the steps below were transcribed by reading that diagram rather than from machine-extracted text. The logic and wait times are consistent with the rest of the document, but if this sequence is safety/mission-critical, cross-check the numbers directly against the Figure 7 image on page 30 of the source PDF.

On power-on: internal POR releases reset once supplies are in spec, then component reads NVM (calibration/config) into volatile registers (25 ms wait — see TMODE), then goes to **power-down mode**. An SPI command is required to enter **run/operation mode**.

**Full sequence (Figure 7, both ASICs):**
1. Wait until V3p3A/V3p3D in spec (3.0–3.6 V).
2. Wait 25 ms for NVM read.
3. Set DUE operation mode **twice** (write Mode reg 19h with OP_MODE=0), the two writes must be <1 ms apart (DUE requires 2× write for Rate Y startup). Set UNO operation mode once.
4. Wait 70 ms (initial startup).
5. Set UNO configuration (data filter, flag filter) via G_FILT_DYN/A_FILT_DYN.
6. Reset DUE: write `0001h` to ResCTRL (18h), or use EXTRESN pin. Wait 25 ms for NVM read.
7. Set DUE operation mode again (2×). Wait 1 ms.
8. Set DUE configuration (data filter, flag filter).
9. Wait 405 ms (46/300 Hz filter) or 525 ms (13/20 Hz filter) for startup self-tests (GYRO/ACC startup self-test, shield detection, CCM BIST, parallel tests).
10. Set EOI=1 (End Of Initialization) via ResCTRL (18h).
11. Read Summary Status register (0Eh) **twice** — the first read after startup may incorrectly show FAIL; a 2nd read is required to confirm all status bits = OK ('1'). Wait time between the 1st and 2nd Summary Status reads must be **≥ 2.5 ms**. (Do not confuse this with the unrelated `TDEL_CTRL` field in ResCTRL/18h, §7.13, which governs how long *detailed* status registers stay valid after an S_Sum read once EOI is set.)
12. If Return Status = '01' (OK) → system in operation, data ready.
   If not, and this is the first attempt → restart UNO & DUE (reset both via 18h or EXTRESN, wait 25 ms NVM, re-set operation modes ×2 for DUE + config, wait 45 ms adjusted restart duration ~500 ms) and retry once.
   If still failing on 2nd attempt → **system in FAILURE mode**.

**Filter default:** lowest available low-pass filter (13 Hz) is used by default; other filters selected via SPI (G_FILT_DYN 16h / A_FILT_DYN 1Ah).

---

## 6. SPI Interface Protocol

- Two independent physical SPI buses (UNO, DUE), slave-only, 4-wire (3-wire not supported).
- **Off-frame (pipelined) protocol**: each 32-bit transfer's MOSI carries the *next* request while MISO returns the response to the *previous* request. The first MISO response after reset is undefined and must be discarded.
- Data word length: 32 bits, MSB first.
- SPI Mode 0 (CPOL=0, CPHA=0). CSB falling edge starts frame, rising edge ends it.

### 6.1 General instruction format (32-bit word)

```
Bit:   31  30  29  28  27  26  25  24  23 ... 9   8   7   6 ... 1   0
MOSI: OP5 OP4 OP3 OP2 OP1 OP0  0   0  DI15...DI1 DI0 CRC7...CRC1 CRC0
MISO: OP5 OP4 OP3 OP2 OP1 OP0 RS1 RS0 DO15...DO1 DO0 CRC7...CRC1 CRC0
      |------ Operation Code -----|-RS-|------ Data (16b) ------|--- CRC(8b) ---|
```

| Field | Bits | MOSI | MISO |
|---|---|---|---|
| OP[5:0] | 31:26 | Requested op: OP5=Write(1)/Read(0); OP[4:0]=register address | Performed op (echoed) |
| RS[1:0] | 25:24 | n/a | Return/sensor status |
| D[15:0] | 23:8 | Data to write | Return data |
| CR[7:0] | 7:0 | Checksum of MOSI[31:8] | Checksum of MISO[31:8] |

Unused bits must be 0 (affects CRC calculation).

### 6.2 Return Status (RS) bits (Table 16)
| RS[1] | RS[0] | Meaning |
|---|---|---|
| 0 | 0 | Initialization running |
| 0 | 1 | **Normal operation** of selected channel |
| 1 | 0 | Self-test of selected channel ongoing or not started |
| 1 | 1 | Reserved / non-existing register addressed / error of selected channel or common failure |

Priority (high→low): `10 → 00 → 11 → 01`.

### 6.3 CRC-8 checksum (Table 17)
| Parameter | Value |
|---|---|
| Name | CRC-8 |
| Width | 8 bit |
| Polynomial | 0x1D (X8+X4+X3+X2+1) |
| Init | 0xFF |
| XOR out | 0xFF (invert result) |

Covers the 24 MSBs of the 32-bit word (bits 31:8); the 8 LSBs (CRC field) are excluded.

```c
// Calculate CRC for 24 MSB's of the 32 bit dword
// (8 LSB's are the CRC field and are not included in CRC calculation)
uint8_t CalculateCRC(uint32_t Data)
{
    uint8_t BitIndex;
    uint8_t BitValue;
    uint8_t CRC;

    CRC = 0xFF;
    for (BitIndex = 31; BitIndex > 7; BitIndex--)
    {
        BitValue = (uint8_t)((Data >> BitIndex) & 0x01);
        CRC = CRC8(BitValue, CRC);
    }
    CRC = (uint8_t)~CRC;
    return CRC;
}

static uint8_t CRC8(uint8_t BitValue, uint8_t CRC)
{
    uint8_t Temp;
    Temp = (uint8_t)(CRC & 0x80);
    if (BitValue == 0x01)
    {
        Temp ^= 0x80;
    }
    CRC <<= 1;
    if (Temp > 0)
    {
        CRC ^= 0x1D;
    }
    return CRC;
}
```

**Worked example:** Read RATE register (01h) → SPI[31:8] = `040000h` → CRC[7:0] = `F7h`.

### 6.4 Operations & example SPI frames (Table 15)

| Operation | ASIC | Register | Binary (OP,RS,Data,CRC) | Hex |
|---|---|---|---|---|
| Set Operation mode | DUE/UNO | MODE (19h) | `111001 00 0000000000000000 01100111` | `E4000067h` |
| Select 13Hz filter for RATE | DUE/UNO | G_FILT_DYN (16h) | `110110 00 0000000000000000 01000101` | `D8000045h` |
| Select 46Hz filter for RATE | DUE/UNO | G_FILT_DYN (16h) | `110110 00 0001001000010010 10011110` | `D812129Eh` |
| Select 13Hz filter for ACC | UNO | A_FILT_DYN (1Ah) | `111010 00 0000000000000000 01101101` | `E800006Dh` |
| Select 46Hz filter for ACC | UNO | A_FILT_DYN (1Ah) | `111010 00 0000001000100010 01001000` | `E8022248h` |
| Set EOI bit | DUE/UNO | ResCTRL (18h) | `111000 00 0000000000000010 01011011` | `E000025Bh` |
| Reset via SPI | DUE/UNO | ResCTRL (18h) | `111000 00 0000000000000001 01111100` | `E000017Ch` |
| Read RATE_X | UNO | RATE_X (01h) | `000001 00 0000000000000000 11110111` | `040000F7h` |
| Read RATE_Y | DUE | RATE_Y (03h) | `000011 00 0000000000000000 11111011` | `0C0000FBh` |
| Read RATE_Z | DUE | RATE_Z (01h) | `000001 00 0000000000000000 11110111` | `040000F7h` |
| Read ACC_X | UNO | ACCX (04h) | `000100 00 0000000000000000 11101001` | `100000E9h` |
| Read ACC_Y | UNO | ACCY (05h) | `000101 00 0000000000000000 11101111` | `140000EFh` |
| Read ACC_Z | UNO | ACCZ (06h) | `000110 00 0000000000000000 11100101` | `180000E5h` |
| Read TEMP | DUE/UNO | TEMP (07h) | `000111 00 0000000000000000 11100011` | `1C0000E3h` |
| Read Summary Status | DUE/UNO | S_Sum (0Eh) | `001110 00 0000000000000000 11010101` | `380000D5h` |
| Read Rate Status 1 | DUE/UNO | R_S1 (10h) | `010000 00 0000000000000000 10010001` | `40000091h` |
| Read Rate Status 2 | DUE | R_S2 (11h) | `010001 00 0000000000000000 10010111` | `44000097h` |
| Read Accelerometer Status 1 | UNO | A_S1 (12h) | `010010 00 0000000000000000 10011101` | `4800009Dh` |
| Read Common Status 1 | DUE/UNO | C_S1 (14h) | `010100 00 0000000000000000 10001001` | `50000089h` |
| Read Common Status 2 | DUE/UNO | C_S2 (15h) | `010101 00 0000000000000000 10001111` | `5400008Fh` |

Note: register R/W permission: if EOI=1, only `ResCTRL` bit0 (reset) and `SYS_TEST` (17h) remain writable. R/W to a Reserved register sets RS='11'. Don't write to Factory-Use-Only registers (may permanently damage the component).

---

## 7. Register Map

### 7.1 Address space overview (Table 18)

| Reg (hex) | DUE R/W | DUE Name | UNO R/W | UNO Name |
|---|---|---|---|---|
| 00h | N/A | Reserved | N/A | Reserved |
| 01h | R | RATE_Z (Rz) | R | RATE_X (Rx) |
| 02h | N/A | Reserved | N/A | Reserved |
| 03h | R | RATE_Y (Ry) | N/A | Reserved |
| 04h | N/A | Reserved | R | ACC_X (Ax) |
| 05h | N/A | Reserved | R | ACC_Y (Ay) |
| 06h | N/A | Reserved | R | ACC_Z (Az) |
| 07h | R | TEMP | R | TEMP |
| 08h–0Ah | R | Reserved | N/A | Reserved |
| 0Bh | R | RATE_Z2 (Rz2) | R | RATE_X2 (Rx2) |
| 0Ch | N/A | Reserved | N/A | Reserved |
| 0Dh | R | RATE_Y2 (Ry2) | N/A | Reserved |
| 0Eh | R | S_Sum | R | S_Sum |
| 0Fh | R/W | SCtrl | R/W | SCtrl |
| 10h | R | R_S1 | R | R_S1 |
| 11h | R | R_S2 | R | R_S2 |
| 12h | R | A_S1 | R | A_S1 |
| 13h | N/A | Reserved | N/A | Reserved |
| 14h | R | C_S1 | R | C_S1 |
| 15h | R | C_S2 | R | C_S2 |
| 16h | R/W | G_FILT_DYN | R/W | G_FILT_DYN |
| 17h | N/A | SYS_TEST | R/W | SYS_TEST |
| 18h | R/W | ResCTRL | R/W | ResCTRL |
| 19h | R/W | Mode | R/W | Mode |
| 1Ah | N/A | Reserved | R/W | A_FILT_DYN |
| 1Bh | R | C_ID | R | C_ID |
| 1Ch | R | T_ID2 | R | T_ID2 |
| 1Dh | R | T_ID0 | R | T_ID0 |
| 1Eh | R | T_ID1 | R | T_ID1 |
| 1Fh | R/W | SelBnk | R/W | SelBnk |

### 7.2 Sensor Data Block (Table 19)
| ASIC | Addr | Bits | Register | R/W | Description |
|---|---|---|---|---|---|
| UNO | 01h | [15:0] | RATE X | R | Rate output, 2's complement |
| DUE | 03h | [15:0] | RATE Y | R | Rate output, 2's complement |
| DUE | 01h | [15:0] | RATE Z | R | Rate output, 2's complement |
| UNO | 04h | [15:0] | ACC_X | R | X-axis accel, 2's complement |
| UNO | 05h | [15:0] | ACC_Y | R | Y-axis accel, 2's complement |
| UNO | 06h | [15:0] | ACC_Z | R | Z-axis accel, 2's complement |
| DUE/UNO | 07h | [15:0] | TEMP | R | Temperature, 2's complement |

**Rate conversion:** `angular_rate[°/s] = raw_LSB(2's complement) / sensitivity` where sensitivity = 160 LSB/(°/s) typ (125 dps version).
Example: RATE_X read `05FFE08Bh` → data field `FFE0h` = −32 decimal → −32/160 = **−0.2 °/s**.

**Accel conversion:** `accel[g] = raw_LSB(2's complement) / sensitivity` where sensitivity = 4905 LSB/g typ.
Example: ACC_Z read `1900DC0Eh` → data field `00DCh` = 220 decimal → 220/4905 = **0.049 g**.

**Temp conversion:** `Temperature[°C] = 25 + (TEMP/30)`.
Example: TEMP read `1DFE6F4Eh` → data field `FE6Fh` = −401 decimal → 25 + (−401/30) = **+11.6 °C**.

### 7.3 Sensor Status Block & Control overview (Table 20)
| Addr | Register | Bits | R/W | Description |
|---|---|---|---|---|
| 0Eh | S_Sum | 16 | R | Summary Status |
| 0Fh | SCtrl | 16 | R/W | Safe control |
| 10h | R_S1 | 16 | R | Rate Status 1 |
| 11h | R_S2 | 16 | R | Rate Status 2 |
| 12h | A_S1 | 16 | R | Accelerometer Status 1 |
| 14h | C_S1 | 16 | R | Common Status 1 |
| 15h | C_S2 | 16 | R | Common Status 2 |
| 16h | G_FILT_DYN | 16 | R/W | Gyro filter/dynamic control |
| 18h | ResCtrl | 16 | R/W | Reset control + EOI |
| 19h | Mode | 16 | R/W | Mode register |
| 1Ah | A_FILT_DYN | 16 | R/W | Accel filter/dynamic control (UNO only) |

> R/W note: register content can always be read; it can be over-written in normal operation only if EOI=0. Once EOI=1, only ResCTRL bit0 (HardReset) and SYS_TEST (17h) remain writable.

---

### 7.4 Summary Status Register — S_Sum (0Eh) — Read-only

Indicates saturation or failure; failure = OK flag set to 0, latched until register read.

| Bit | DUE Name | UNO Name | Description |
|---|---|---|---|
| D15 | S_OK_C | S_OK_C | Status summary flag for common blocks |
| D14 | S_OK_Ry_F | Reserved | Status summary flag for Ry (expect *_OS too) |
| D13 | S_OK_Ry_OS | Reserved | Saturation flags summary for Ry |
| D12 | S_OK_Rz_F | S_OK_Rx_F | Status summary flag for Rz/Rx |
| D11 | S_OK_Rz_OS | S_OK_Rx_OS | Saturation flags summary for Rz/Rx |
| D10 | Reserved | S_OK_Ax_F | Status summary flag for Ax |
| D9 | Reserved | S_OK_Ax_OS | Saturation flags summary for Ax |
| D8 | Reserved | Reserved | – |
| D7 | Reserved | S_OK_Ay_F | Status summary flag for Ay |
| D6 | Reserved | S_OK_Ay_OS | Saturation flags summary for Ay |
| D5 | Reserved | Reserved | – |
| D4 | Reserved | S_OK_Az_F | Status summary flag for Az |
| D3 | Reserved | S_OK_Az_OS | Saturation flags summary for Az |
| D2 | Reserved | Reserved | – |
| D1 | STUP_OK_Rz | STUP_OK_Rx | Startup functions status flag for Rz/Rx |
| D0 | STUP_OK_Ry | Reserved | Startup functions status flag for Ry |

---

### 7.5 Safe Control Register — SCtrl (0Fh) — R/W, all bits writable

| Bit | DUE Name | UNO Name | Description |
|---|---|---|---|
| D15:D12 | RATEsat[3:0] | RATEsat[3:0] | Saturation-flag filter counter for all rate outputs. Time window between RATEsat×LSB and (RATEsat+1)×LSB, 1 LSB = 32 × F_prim_period |
| D11:D8 | Reserved | ACCsat[3:0] | Saturation-flag filter counter for all accel outputs, same LSB definition |
| D7 | Stat_rmode | Stat_rmode | Status read mode: 0 = reading S_Sum clears Bank0 status regs to 1 after 36×F_prim_period delay; 1 = clears without delay. After reading S_Sum, wait ≥2.5 ms before writing Stat_rmode=1. Reading any other status register always clears without delay regardless of this bit. 36×F_prim_period ≈ 2.02–2.28 ms (UNO), 1.77–1.97 ms (DUE) |
| D6 | St_req_ccm | St_req_ccm | Request CCM self-test (write 1; auto-clears when done) |
| D5 | St_req_temp | St_req_temp | Request Temperature start-up self-test (write 1; auto-clears) |
| D4 | St_req_Ry | Reserved | Request Ry start-up self-test (DUE only) |
| D3 | St_req_Rz | St_req_Rx | Request Rz/Rx start-up self-test |
| D2 | Reserved | St_req_ACC | Request ACC channels start-up self-test (UNO only) |
| D1 | St_req_sys | St_req_sys | Request system start-up self-test |
| D0 | Sat_mode | Sat_mode | Saturation flag mode: 0 (default) = error flag set on saturation, requires SPI-read ack before clearing; 1 = flag auto-clears when saturation stops |

---

### 7.6 Rate Status 1 Register — R_S1 (10h) — Read-only

DUE reports Rz status; UNO reports Rx status (same bit layout, suffix differs).

| Bit | DUE Name | UNO Name | Description (1 = OK) |
|---|---|---|---|
| D15 | OVS_ANAz_OK | OVS_ANAx_OK | Overflow saturation status, analog part |
| D14 | OVS_DIGz_OK | OVS_DIGx_OK | Overflow saturation status, digital part |
| D13 | CST_Rz_OK | CST_Rx_OK | Continuous self-test OK |
| D12 | GST_Rz_OK | GST_Rx_OK | Start-up self-test OK |
| D11 | PLL_OSC_FRz_OK | PLL_OSC_FRx_OK | PLL freq. vs RC oscillator comparison OK |
| D10 | MON_VCMF_Rz_OK | MON_VCMF_Rx_OK | CCM Monitoring: VCMF OK |
| D9 | MON_PRIz_FE_OK | MON_PRIx_FE_OK | Primary front end OK |
| D8 | MON_SECz_FE_OK | MON_SECx_FE_OK | Secondary front end OK |
| D7 | MON_DRV_Rz_OK | MON_DRV_Rx_OK | CCM Monitoring: drive path OK |
| D6 | MON_AGC_Rz_OK | MON_AGC_Rx_OK | CCM Monitoring: AGC OK |
| D5 | MON_QC_Rz_OK | MON_QC_Rx_OK | CCM Monitoring: quadrature compensation path OK |
| D4 | MON_LPF_Rz_OK | MON_LPF_Rx_OK | CCM Monitoring: force-feedback path OK |
| D3 | Reserved | Reserved | – |
| D2 | MEMS_INT_Rz_OK | MEMS_INT_Rx_OK | MEMS interface OK |
| D1 | Rz_PP_CRC_OK | Rx_PP_CRC_OK | Filter coefficient CRC OK |
| D0 | Rz_PRI_OK | Rx_PRI_OK | Primary channel OK |

---

### 7.7 Rate Status 2 Register — R_S2 (11h) — Read-only

DUE reports **Ry** status; **UNO: all bits Reserved** (no second gyro channel).

| Bit | DUE Name | Description (1 = OK) |
|---|---|---|
| D15 | OVS_ANAy_OK | Overflow saturation, analog part (Ry) |
| D14 | OVS_DIGy_OK | Overflow saturation, digital part (Ry) |
| D13 | CST_Ry_OK | Continuous self-test OK |
| D12 | GST_Ry_OK | Start-up self-test OK |
| D11 | PLL_OSC_FRy_OK | PLL vs RC oscillator OK |
| D10 | MON_VCMF_Ry_OK | CCM: VCMF OK |
| D9 | MON_PRIy_FE_OK | Primary front end OK |
| D8 | MON_SECy_FE_OK | Secondary front end OK |
| D7 | MON_DRV_Ry_OK | CCM: drive path OK |
| D6 | MON_AGC_Ry_OK | CCM: AGC OK |
| D5 | MON_QC_Ry_OK | CCM: quadrature comp. path OK |
| D4 | MON_LPF_Ry_OK | CCM: force-feedback path OK |
| D3 | Reserved | – |
| D2 | MEMS_INT_Ry_OK | MEMS interface OK |
| D1 | DSP2_OK | DSP2 OK |
| D0 | Ry_PRI_OK | Primary channel OK |

---

### 7.8 Accelerometer Status 1 Register — A_S1 (12h) — Read-only

**DUE: all bits Reserved.** UNO reports accelerometer status.

| Bit | UNO Name | Description (1 = OK) |
|---|---|---|
| D15 | OVS_ANA_Ax_OK | Overflow saturation, Ax analog |
| D14 | OVS_ANA_Ay_OK | Overflow saturation, Ay analog |
| D13 | OVS_ANA_Az_OK | Overflow saturation, Az analog |
| D12 | OVS_DIG_Ax_OK | Overflow saturation, Ax digital |
| D11 | OVS_DIG_Ay_OK | Overflow saturation, Ay digital |
| D10 | OVS_DIG_Az_OK | Overflow saturation, Az digital |
| D9:D7 | Reserved | – |
| D6 | CST_Ax_OK | Continuous self-test Ax OK |
| D5 | CST_Ay_OK | Continuous self-test Ay OK |
| D4 | CST_Az_OK | Continuous self-test Az OK |
| D3 | ACC_PP_CRC_OK | ACC filter coefficient CRC OK |
| D2 | STU_MD_STAT_OK | ACC start-up mass-deflection self-test OK |
| D1 | STAT_SD_ACC_OK | ACC shield-detection self-test OK |
| D0 | Reserved | – |

---

### 7.9 Common Status 1 Register — C_S1 (14h) — Read-only

Same layout on both ASICs (bit meaning differs slightly per column).

| Bit | DUE Name | UNO Name | Description |
|---|---|---|---|
| D15 | EOI_state | EOI_state | End of Initialization: startup sequence completed OK (1=OK) |
| D14 | STDIS_C | STDIS_C | All common startup self-tests inactive(1)/active(0) |
| D13 | STDIS_Ry | Reserved | Ry startup self-tests inactive(1)/active(0) |
| D12 | STDIS_Rz | STDIS_Rx | Rz/Rx startup self-tests inactive(1)/active(0) |
| D11 | Reserved | STDIS_A | ACC startup self-tests disabled(1)/enabled(0) |
| D10 | BIST_DSP2_DONE | BIST_DSP2_DONE | DSP2 BIST done (valid only for DUE; constant 1 on UNO) |
| D9 | MCLK2_stat | Reserved | Comparison of PLL signals from gyro1 vs gyro2 |
| D8:D6 | Reserved | Reserved | – |
| D5 | Nmode_OK | Nmode_OK | Normal mode OK (1=normal; 0=any test mode active) |
| D4 | NVM_START_OK | NVM_START_OK | NVM module OK |
| D3 | HV_OK | HV_OK | HV generator OK |
| D2 | CRC_SPI_OK | CRC_SPI_OK | SPI CRC comparison OK |
| D1 | CRC_REG_OK | CRC_REG_OK | Register map CRC OK |
| D0 | CRC_NVM_OK | CRC_NVM_OK | NVM CRC OK |

---

### 7.10 Common Status 2 Register — C_S2 (15h) — Read-only

Identical layout on DUE and UNO.

| Bit | Name (both ASICs) | Description (1=OK) |
|---|---|---|
| D15 | MON_TEMP_OK | CCM Monitoring: temperature sensor OK |
| D14 | MON_FAIL_P_OK | CCM Monitoring: FAIL_P OK |
| D13 | MON_V3p3D_OK | CCM Monitoring: V3p3D supply OK |
| D12 | MON_V3p3A_OK | CCM Monitoring: V3p3A supply OK |
| D11 | MON_DVBG_OK | CCM Monitoring: DVBG OK |
| D10 | MON_FAIL_N_OK | CCM Monitoring: FAIL_N OK |
| D9 | MON_VDDD_OK | CCM Monitoring: VDDD OK |
| D8 | TEMP_ANA_OK | Temperature analog data path OK |
| D7 | CCM_CAL_OK | CCM internal monitoring OK |
| D6:D4 | Reserved | – |
| D3 | TEMP_CALC_OK | Temperature calculation status OK |
| D2 | DSP1_CLOCK_OK | DSP1 clock OK |
| D1 | DSP1_OK | DSP1 OK |
| D0 | ST_STAT_TEMP | Temperature start-up self-test status OK |

---

### 7.11 Gyro Filter & Dynamic Control — G_FILT_DYN (16h) — R/W

DUE controls **both Rz/Rz2 (bits 15:8) and Ry/Ry2 (bits 6:0)**; UNO controls **only Rx/Rx2 (bits 15:8)** — bits 6:0 Reserved on UNO.

| Bit | DUE Name | UNO Name | Description |
|---|---|---|---|
| D15 | Reserved | Reserved | – |
| D14 | Rz2_DYN | Rx2_DYN | Rz2/Rx2 output dynamic range: 0=nominal (204.8 dps @125dps ver / 409.6 dps @300dps ver); 1=nominal/2. Post-filter digital-gain shift; LSB step & quantization noise halved. |
| D13:D11 | Rz2_FILT[2:0] | Rx2_FILT[2:0] | Rz2/Rx2 filter: `000`=13Hz, `001`=20Hz, `010`=46Hz, `011`=200Hz (not recommended), `1xx`=300Hz (3rd-order) |
| D10:D8 | Rz_FILT[2:0] | Rx_FILT[2:0] | Rz/Rx filter, same encoding |
| D7 | Reserved | Reserved | – |
| D6 | Ry2_DYN | Reserved | Ry2 dynamic range, same encoding as Rz2_DYN (DUE only) |
| D5:D3 | Ry2_FILT[2:0] | Reserved | Ry2 filter, same encoding (DUE only) |
| D2:D0 | Ry_FILT[2:0] | Reserved | Ry filter, same encoding (DUE only) |

---

### 7.12 SYS_TEST Register (17h) — R/W, NOT locked by EOI

16-bit read/write scratch register to verify device accessibility / multi-device SPI bus wiring; not gated by EOI. Sequence: write data → read back on next transfer → dummy command returns the written value (off-frame pipelining, see §6.1/§6 general).

---

### 7.13 Reset Control Register — ResCTRL (18h) — R/W

Same layout on DUE and UNO.

| Bit | Name (both ASICs) | Description |
|---|---|---|
| D15:D6 | Reserved | Write zero |
| D5:D4 | TDEL_CTRL[1:0] | S_Sum read delay (TDEL) — only relevant if Stat_rmode=0 in SCtrl (0Fh): `00`→1.7–2.4 ms, `01`→0.7–1.0 ms, `10`→0.3–0.5 ms, `11`→0.089–0.123 ms. TDEL starts on S_Sum read; detailed status registers must be read within TDEL or failure info is lost. |
| D3:D2 | Reserved | Write zero |
| D1 | EOI | **End of Initialization**: locks all R/W registers (except HardReset bit here) and SYS_TEST (17h); reset required to clear. Settable to 1 only once all startup self-tests are complete. 1=normal operation; 0=init state, SPI RS cannot be 01. |
| D0 | HardReset | Write 1 to reset the ASIC; auto-clears to 0 |

---

### 7.14 Mode Register — Mode (19h) — R/W

| Bit | DUE Name | UNO Name | Description |
|---|---|---|---|
| D15 | Reserved | Dis_CST_ACC | Disable CST of ACC channel (1=disable, not useful with EOI). Offset/sensitivity shift in Az becomes visible if disabled |
| D14 | Reserved | Reserved | – |
| D13 | OP_MODE | OP_MODE | 1 = Low power mode (default after reset); 0 = Normal operation mode |
| D12 | Dis_Ry | Reserved | Disable Y-Gyro (1=disable; DUE only) |
| D11 | Dis_Rz | Dis_Rx | Disable Z-Gyro (DUE) / X-Gyro (UNO) |
| D10 | Reserved | Dis_ACC | Disable accelerometer (UNO only) |
| D9 | Dis_ACCT | Dis_ACCT | Disable accelerometer + temp sensor |
| D8 | Dis_CST_Ry | Reserved | Disable CST of Ry (DUE only) |
| D7 | Dis_CST_Rz | Dis_CST_Rx | Disable CST of Rz (DUE) / Rx (UNO) |
| D6 | Miso_SR | Miso_SR | MISO slew-rate: 0=CMISO 10…85 pF (default), 1=CMISO 70…200 pF |
| D5:D3 | Unlock_ASM[2:0] | Unlock_ASM[2:0] | Unlock ASIC State Machine (test-mode entry). Write sequence: `010`, then `001`, then `100` (see §7.16 test-mode procedure) |
| D2:D0 | Stat_ASM[2:0] | Stat_ASM[2:0] | ASIC state-machine status (read after each Unlock_ASM write): `000`=no/invalid code, `010`=code 010 accepted, `011`=code 001 accepted, `111`=code 100 accepted → all test registers writable. Only Power-On-Reset re-locks. |

Disable bits (Dis_*) are "not useful with EOI function" — i.e. intended for pre-EOI configuration.

> ⚠️ **Source inconsistency:** the datasheet's own bit-description text for D12/D11 reads "Disable Z-Gyro" next to the `Dis_Ry` bit name and "Disable X-Gyro" next to `Dis_Rz` (DUE column) — i.e. the prose description doesn't match the bit's own name for the DUE ASIC. Cross-checking against the register map (§7.1: DUE reg 03h=RATE_Y, reg 01h=RATE_Z) and the SCtrl (0Fh) bit names (§7.5: `St_req_Ry`/`St_req_Rz`), the bit **names** (`Dis_Ry` disables Ry/Y-gyro, `Dis_Rz` disables Rz/Z-gyro) are internally consistent with the rest of the document, so the table above follows the bit names. Verify against Murata's errata/latest revision if this bit is safety-relevant.

---

### 7.15 ACC Filter & Dynamic Control — A_FILT_DYN (1Ah) — R/W, **UNO only** (all Reserved on DUE)

| Bit | UNO Name | Description |
|---|---|---|
| D15:D12 | Reserved | – |
| D11 | Ax_DYN | Ax dynamic range: 0=nominal (6.7g default, 6g meas. range); 1=nominal/4. Post-filter digital gain; LSB step & quant. noise = 1/4 |
| D10:D8 | Ax_FILT[2:0] | Ax filter: `000`=13Hz,`001`=20Hz,`010`=46Hz,`011`=200Hz,`1xx`=300Hz (3rd order) |
| D7 | Ay_DYN | Ay dynamic range, same encoding as Ax_DYN |
| D6:D4 | Ay_FILT[2:0] | Ay filter, same encoding |
| D3 | Az_DYN | Az dynamic range, same encoding |
| D2:D0 | Az_FILT[2:0] | Az filter, same encoding |

---

### 7.16 Component ID / Traceability registers (Read-only)

- **Component ID — C_ID (1Bh):** 16-bit `CID[15:0]` value.
- **Traceability 2 — T_ID2 (1Ch):** bits `[11:8]`=`TID2[3:0]` (F: hex running number/FAB code); bits `[7:6]`=`ASIC_type[1:0]` (`01`=UNO, `10`=DUE); bits `[5:3]`=`ASIC_ver[2:0]` (`001`=ver1, `010`=ver2); rest Reserved.
- **Traceability 0 — T_ID0 (1Dh):** `TID0[15:0]` = 16-bit hex running number (HHHH).
- **Traceability 1 — T_ID1 (1Eh):** `TID1[15:0]` = 16-bit unsigned int = DDDYY (production day 1–366, year 18–99).

**Serial number format:** `DDDYYFHHHHH00` where DDD=day, YY=year, F=FAB/hex running nibble, HHHH=hex running number, `H00`=fixed suffix for SCHA63T-K01 family (not stored in NVM).
Example: T_ID2 nibble→`0`; T_ID0=`0001 0100 1001 0111`→`1497`; T_ID1=`1000101001011001`(16-bit uint)→`35417`(interpreted as DDDYY). Result: **`3541701497H00`**.

### 7.17 Test-Mode Procedure for Reading Cross-Axis Compensation Terms (§3.6.1 of source)

1. After step 3 (or 4/5/6) of the startup sequence, unlock test mode: write Mode(19h) with `Unlock_ASM=010`, read Mode; write `Unlock_ASM=001`, read Mode; write `Unlock_ASM=100`, read Mode; dummy read to get final response. Verify `Stat_ASM[2:0]='111'`.
2. Change bank to 5: write `05h` to address `1Fh` (SelBnk).
3. Read cross-axis terms per Table 5 (register map in §8 below).
4. Save cross-axis terms to MCU.
5. Exit test mode: power-off, or SPI reset via ResCTRL (18h), or EXTRESN pin toggle.
6. Return to step 1 of the normal startup sequence (§5).

⚠️ **Writing to an unspecified register after test-mode access may permanently damage the component.**

---

## 8. Cross-Axis Compensation

The DUE ASIC (bank 05h) stores cross-axis compensation and offset fine-tuning terms for both rate and accelerometer channels. Apply compensation on the host MCU (not automatically applied by the sensor).

### 8.1 Rate cross-axis compensation

```
Ω_real = C · (Ω_meas − Ω_offs)

C = | c_xx  c_xy  c_xz |      Ω_real = (Ω_rx, Ω_ry, Ω_rz)^T   (compensated rate vector)
    | c_yx  c_yy  c_yz |      Ω_meas = (Ω_mx, Ω_my, Ω_mz)^T   (measured rate vector, from component)
    | c_zx  c_zy  c_zz |      Ω_offs = (Ω_ox, Ω_oy, Ω_oz)^T   (rate offset vector — NOT stored in NVM;
                                                                 system-level zeroing after PCB assembly
                                                                 is recommended)
```

### 8.2 Accelerometer cross-axis compensation

```
a_real = B · (a_meas − a_offs)

B = | b_xx  b_xy  b_xz |      a_real = (a_rx, a_ry, a_rz)^T
    | b_yx  b_yy  b_yz |      a_meas = (a_mx, a_my, a_mz)^T   (measured, from component)
    | b_zx  b_zy  b_zz |      a_offs = (a_ox, a_oy, a_oz)^T   (NOT stored in NVM; system-level zeroing
                                                                 after assembly recommended)
```

> Note: sensing-element axes are mechanically independent; cross-axis compensation does not affect axis independency as long as no axis is saturated.

### 8.3 Cross-Axis Compensation Register Map (Table 5)

All registers live in **DUE ASIC, Bank 05h**. Range: 2's complement, −128…127 LSB.

| Term | Addr (hex) | Register | Floating-point conversion |
|---|---|---|---|
| c_xx | 0Bh | ACC_DC1[7:0] | `1 + value/4096` |
| c_xy | 0Bh | ACC_DC1[15:8] | `value/4096` |
| c_xz | 13h | ACC_DC9[7:0] | `value/4096` |
| c_yx | 13h | ACC_DC9[15:8] | `value/4096` |
| c_yy | 14h | ACC_DC10[7:0] | `1 + value/4096` |
| c_yz | 14h | ACC_DC10[15:8] | `value/4096` |
| c_zx | 15h | ACC_DC11[7:0] | `value/4096` |
| c_zy | 15h | ACC_DC11[15:8] | `value/4096` |
| c_zz | 16h | ACC_DC12[7:0] | `1 + value/4096` |
| b_xx | 16h | ACC_DC12[15:8] | `1 + value/4096` |
| b_xy | 17h | ACC_DC13[7:0] | `value/4096` |
| b_xz | 17h | ACC_DC13[15:8] | `value/4096` |
| b_yx | 18h | ACC_DC14[7:0] | `value/4096` |
| b_yy | 18h | ACC_DC14[15:8] | `1 + value/4096` |
| b_yz | 1Bh | ACC_MD1[7:0] | `value/4096` |
| b_zx | 1Bh | ACC_MD1[15:8] | `value/4096` |
| b_zy | 1Ch | ACC_MD2[7:0] | `value/4096` |
| b_zz | 1Ch | ACC_MD2[15:8] | `1 + value/4096` |

To read this bank, enter test mode per §7.17, set `SelBnk=05h`, then read registers via normal SPI read operations.

---

## 9. Application Circuit / External Components

### 9.1 Decoupling capacitors (Table 47)

| Symbol | Between | Min | Nom | Max | Unit |
|---|---|---|---|---|---|
| C5, C6 | V3p3D ↔ GNDD | 0.7 | 1 | 1.3 | µF (ESR <100 mΩ @1MHz) |
| C8, C9 | V3p3A ↔ GNDA | 70 | 100 | 130 | nF (ESR <100 mΩ @1MHz) |
| C11, C12 | VDDD ↔ GNDD | 0.7 | 1 | 1.3 | µF (ESR <100 mΩ @1MHz) |
| C13, C14 | VDDA ↔ GNDA | 0.7 | 1 | 1.3 | µF (ESR <100 mΩ @1MHz) |

One capacitor pair per ASIC (DUE: C5/C8/C11/C13; UNO: C6/C9/C12/C14 — see Figure 15 in source PDF for exact placement). VDDD/VDDA are internally regulated outputs — external decoupling capacitors are mandatory per pin descriptions.

### 9.2 PCB layout guidelines

- Place SMD decoupling capacitors directly adjacent to the component on the top layer.
- Provide a solid ground plane under the component; connect every ground pin directly to it.
- Signal lines of this component (SCHA6xx series) may be routed under the component on the top layer **only if** sampling rate follows the recommended values (§9.3) — see SPI Crosstalk Optimization.
- Use individual analog/digital power-supply lines with individual decoupling capacitors where possible. If sharing one power line, use a star-shaped separation point between the regulated supply and the decoupling capacitors.
- Keep all routing as low-resistance as possible.
- Two reference layouts exist: 1-SPI-interface (both ASICs sharing a single external SPI/MCU connection scheme, saves PCB space) and 2-SPI-interface (independent buses, provides system redundancy).

### 9.3 SPI Crosstalk Optimization

Digital SPI activity can couple into the gyro MEMS signal through internal electrical coupling. The critical harmonics coincide with F_prim (primary MEMS drive frequency).

**Critical F_prim ranges:**
| MEMS | F_prim_min | F_prim_typ | F_prim_max |
|---|---|---|---|
| Z-gyro | 18.3 kHz | 19.3 kHz | 20.3 kHz |
| X,Y-gyro | 15.8 kHz | 16.8 kHz | 17.8 kHz |

**Recommended SPI sampling frequencies (Table 48):**
| Frequency (kHz) | Tolerance (Hz) |
|---|---|
| 2.58 | ±26 |
| 3.01 | ±30 |
| 3.61 | ±36 |
| 4.51 | ±45 |
| 6.02 | ±60 |

Choose sample rate + duty cycle (CSB low time / sampling period) such that no harmonic of the sample rate lands inside the F_prim critical range (see Table 50 in source PDF for the harmonic×duty-cycle grid — e.g. fs=3.01 kHz with the 6th harmonic at 18.05 kHz sits just below the 18.3 kHz Z-gyro minimum; duty cycle ≈16.7% minimizes that 6th-harmonic contribution). Keep sample-frequency variation < 1% to avoid drifting a "safe" harmonic into the critical band, e.g. `(1+1%) × 3.01 = 18.2 kHz < 18.3 kHz` (Z-gyro F_prim min).

### 9.4 Mechanical robustness

Component is sensitive to acoustic disturbance, especially near F_prim, due to MEMS resonance. Evaluate this in the target system/enclosure. See Murata "Technical Note 96 — Assembly instructions for SCHA63T-K01" for coating/assembly guidance.

---

## 10. Known Bugs

No entries listed in Rev. 2 of this datasheet (table present but empty).

---

## 11. Document History

| Rev | Date | Change | Author |
|---|---|---|---|
| 1 | 2021-02-01 | Final | MAA, VIVI, MAKI |
| 2 | 2021-06-07 | Order codes added; package outline drawing updated; Figure 2 SCK line updated; Figure 18 removed | VIVI, MAKI |

Authors: Kimmo Törmälehto, Yoshitaka Kato, Ville Vilja, Tomoaki Omoda, Masayuki Kimura, Matti Aaltonen (PDT/Lead Engineer, Product Manager). Approved/electronically signed by Ville Nurmiainen (MURATAFI\VNU), 2021-11-28.

---

## 12. Implementation Checklist for a Driver

1. Implement CRC-8 (poly 0x1D, init 0xFF, xor-out 0xFF) over bits [31:8] of every 32-bit frame — see §6.3.
2. Treat UNO and DUE as two independent SPI slaves/state machines; every step in §5 must be done on **both**.
3. Respect the **off-frame pipeline**: the response to command N arrives in the transfer of command N+1; discard the first (post-reset) response.
4. Follow the exact power-up sequence (§5), including the DUE double operation-mode write (<1 ms apart) and the mandatory double read of S_Sum before trusting `Return Status`.
5. After EOI=1, only ResCTRL bit0 and SYS_TEST are writable — configure filters/mode/disable-bits **before** setting EOI.
6. Use §6.4 table's exact hex frames to validate a new SPI/CRC implementation bit-for-bit.
7. Convert raw data using: rate = raw/160 [°/s], accel = raw/4905 [g], temp = 25+raw/30 [°C] (typ. sensitivities; use per-unit calibrated sensitivity from Table 2/3 if available).
8. If bandwidth/noise is critical, pick an SPI sample rate from Table 48 (§9.3) to avoid gyro/SPI crosstalk.
9. Optionally implement cross-axis compensation using Bank 05h coefficients (§8) — requires the test-mode unlock sequence (§7.17) once to read them out, then apply the 3×3 matrix in application code (values not needed again unless recalibrating).
