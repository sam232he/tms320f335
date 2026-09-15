---
part_number: ACS712
manufacturer: Allegro MicroSystems
component_type: Hall-effect linear current sensor IC
package: SOIC-8 (suffix LC)
datasheet_revision: 22
datasheet_date: 2024-02-13
source_doc: ACS712-DS, Rev. 22, MCO-0000197
not_for_automotive_use: true
not_for_life_support_use: true
---

# ACS712 — Hall-Effect-Based Linear Current Sensor IC

## Quick Reference

| Parameter | Value |
|---|---|
| Function | Fully integrated Hall-effect linear current sensor with isolated primary conductor |
| Supply voltage | 5.0 V (4.5–5.5 V) single supply |
| Output type | Analog voltage, ratiometric, proportional to AC or DC primary current |
| Current sensing ranges available | ±5 A, ±20 A, ±30 A (variant-dependent) |
| Sensitivity range | 66–185 mV/A (variant-dependent) |
| Isolation | 2.4 kV_RMS (pins 1–4 to pins 5–8) |
| Primary conductor resistance | 1.2 mΩ typ. |
| Bandwidth | 80 kHz (–3 dB, C_F = 1 nF) |
| Rise time | ⚠ source inconsistency: Features list states 5 µs; Electrical Characteristics table states 3.5 µs typ. (I_P=I_P(max), T_A=25°C, C_OUT open). Use 3.5 µs as the characterized value. |
| Package | 8-pin SOIC, low profile |
| Total output error | ±1.5% typ. at T_A = 25°C |
| Automotive use | NOT intended for automotive applications |

---

## 1. Description

The ACS712 senses AC or DC current by passing it through an internal low-resistance copper conductor. The resulting magnetic field is sensed by a nearby chopper-stabilized BiCMOS Hall IC and converted into a proportional analog output voltage. The current-sampling path (pins 1–2 to pins 3–4) is electrically isolated from the signal leads (pins 5–8), enabling isolated current sensing without opto-isolators.

Typical applications: motor control, load detection/management, switch-mode power supplies, overcurrent fault protection.

Output slope: positive (output > V_IOUT(Q)) when current flows from pins 1&2 → pins 3&4.

**Not intended for automotive applications.**

---

## 2. Pinout

| Pin | Name | Description |
|---|---|---|
| 1 | IP+ | Current-sampling terminal (fused internally with pin 2) |
| 2 | IP+ | Current-sampling terminal (fused internally with pin 1) |
| 3 | IP− | Current-sampling terminal (fused internally with pin 4) |
| 4 | IP− | Current-sampling terminal (fused internally with pin 3) |
| 5 | GND | Signal ground |
| 6 | FILTER | External capacitor input; sets output bandwidth via internal RC (R_F(INT) = 1.7 kΩ typ.) |
| 7 | VIOUT | Analog output signal |
| 8 | VCC | Device power supply (5 V) |

Package orientation (top view, 8-SOIC):
```
IP+ [1]        [8] VCC
IP+ [2]        [7] VIOUT
IP- [3]        [6] FILTER
IP- [4]        [5] GND
```

Standard application circuit: VCC (pin 8) → +5 V with 0.1 µF bypass cap (C_BYP) to ground; FILTER (pin 6) → 1 nF cap (C_F) to ground; VIOUT (pin 7) is the sensed output.

---

## 3. Part Number / Selection Guide

| Part Number | Variant | Packing | T_A Range (°C) | Current Range I_P (A) | Sensitivity Typ. (mV/A) |
|---|---|---|---|---|---|
| ACS712ELCTR-20A-S | -S (lead-free, SnAg solder bumps) | Tape & reel, 3000 pcs/reel | −40 to 85 | ±20 | 100 |
| ACS712ELCTR-30A-S | -S (lead-free, SnAg solder bumps) | Tape & reel, 3000 pcs/reel | −40 to 85 | ±30 | 66 |
| ACS712ELCTR-05B-T | -T (Pb-contained solder bumps) | Tape & reel, 3000 pcs/reel | −40 to 85 | ±5 | 185 |
| ACS712ELCTR-20A-T | -T (Pb-contained solder bumps) | Tape & reel, 3000 pcs/reel | −40 to 85 | ±20 | 100 |
| ACS712ELCTR-30A-T | -T (Pb-contained solder bumps) | Tape & reel, 3000 pcs/reel | −40 to 85 | ±30 | 66 |

Note: -T and -S variants have identical operating performance; -T is RoHS-compliant via Annex III/IV exemptions 7(a), 15, 15(a) of Directive 2011/65/EU.

---

## 4. Absolute Maximum Ratings

| Characteristic | Symbol | Notes | Rating | Units |
|---|---|---|---|---|
| Supply voltage | V_CC | — | 8 | V |
| Reverse supply voltage | V_RCC | — | −0.1 | V |
| Output voltage | V_IOUT | — | 8 | V |
| Reverse output voltage | V_RIOUT | — | −0.1 | V |
| Output current, source | I_IOUT(Source) | — | 3 | mA |
| Output current, sink | I_IOUT(Sink) | — | 10 | mA |
| Overcurrent transient tolerance | I_P | 1 pulse, 100 ms | 100 | A |
| Nominal operating ambient temp | T_A | Range E | −40 to 85 | °C |
| Max junction temperature | T_J(max) | — | 165 | °C |
| Storage temperature | T_stg | — | −65 to 170 | °C |

---

## 5. Isolation Characteristics

| Characteristic | Symbol | Notes | Rating | Unit |
|---|---|---|---|---|
| Withstand voltage | V_ISO | Agency-rated for 60 s per UL 62368-1 (ed. 3); 100% production-tested for 1 s | 2400 | V_RMS |
| Working voltage, basic isolation | V_WVBI | Max approved working voltage per UL 62368-1 (ed. 3) | 420 (V_PK/V_DC) / 297 (V_RMS) | V |
| Impulse withstand voltage | V_IMPULSE | ±5 pulses at 2/min, IEC 61000-4-5, 1.2 µs rise / 50 µs width | 4000 | V_PK |
| Clearance | D_CL | Min. distance through air, IP leads to signal leads | 4 | mm |
| Creepage | D_CR | Min. distance along package body, IP leads to signal leads | 4 | mm |
| Distance through insulation | DTI | Min. internal distance through insulation | 63 | µm |
| Comparative Tracking Index | CTI | Material Group II | 400–599 | V |

Safety standards: CAN/CSA-C22.2 No. 60950-1-03; UL 60950-1:2003; EN 60950-1:2001 (fire and electric shock).

---

## 6. Common Operating Characteristics

Conditions: full T_A range, C_F = 1 nF, V_CC = 5 V (unless noted).

| Characteristic | Symbol | Test Conditions | Min | Typ | Max | Units |
|---|---|---|---|---|---|---|
| Supply voltage | V_CC | — | 4.5 | 5.0 | 5.5 | V |
| Supply current | I_CC | V_CC = 5.0 V, output open | — | 10 | 13 | mA |
| Output capacitance load | C_LOAD | VIOUT to GND | — | — | 10 | nF |
| Output resistive load | R_LOAD | VIOUT to GND | 4.7 | — | — | kΩ |
| Primary conductor resistance | R_PRIMARY | T_A = 25°C | — | 1.2 | — | mΩ |
| Rise time | t_r | I_P = I_P(max), T_A = 25°C, C_OUT open | — | 3.5 | — | µs |
| Frequency bandwidth | f | −3 dB, T_A = 25°C, I_P = 10 A pk-pk | — | 80 | — | kHz |
| Nonlinearity | E_LIN | Over full I_P range | — | 1.5 | — | % |
| Symmetry | E_SYM | Over full I_P range | 98 | 100 | 102 | % |
| Zero-current output voltage | V_IOUT(Q) | Bidirectional, I_P = 0 A, T_A = 25°C | — | V_CC × 0.5 | — | V |
| Power-on time | t_PO | Output reaches 90% steady-state, T_J = 25°C, 20 A on leadframe | — | 35 | — | µs |
| Magnetic coupling | — | 1 G = 0.1 mT | — | 12 | — | G/A |
| Internal filter resistance | R_F(INT) | Forms RC circuit via FILTER pin | — | 1.7 | — | kΩ |

### Thermal Characteristics

| Characteristic | Symbol | Test Conditions | Min | Max | Units |
|---|---|---|---|---|---|
| Operating internal leadframe temp | T_A | E range | −40 | 85 | °C |

| Characteristic | Symbol | Test Conditions | Value | Units |
|---|---|---|---|---|
| Junction-to-lead thermal resistance | R_θJL | Mounted on Allegro ASEK 712 eval board | 5 | °C/W |
| Junction-to-ambient thermal resistance | R_θJA | Mounted on Allegro 85-0322 eval board (includes PCB power) | 23 | °C/W |

---

## 7. Performance Characteristics by Variant

Conditions: T_A = −40°C to 85°C, C_F = 1 nF, V_CC = 5 V (unless noted).

### 7.1 x05B variant (±5 A)

| Characteristic | Symbol | Test Conditions | Min | Typ | Max | Units |
|---|---|---|---|---|---|---|
| Optimized accuracy range | I_P | — | −5 | — | 5 | A |
| Sensitivity | Sens | Full I_P range, T_A = 25°C | 180 | 185 | 190 | mV/A |
| Noise | V_NOISE(PP) | Pk-pk, T_A=25°C, 185 mV/A, C_F=47 nF, C_OUT open, 2 kHz BW | — | 21 | — | mV |
| Zero-current output slope | ΔV_OUT(Q) | T_A = −40°C to 25°C | — | −0.26 | — | mV/°C |
| Zero-current output slope | ΔV_OUT(Q) | T_A = 25°C to 150°C | — | −0.08 | — | mV/°C |
| Sensitivity slope | ΔSens | T_A = −40°C to 25°C | — | 0.054 | — | mV/A/°C |
| Sensitivity slope | ΔSens | T_A = 25°C to 150°C | — | −0.008 | — | mV/A/°C |
| Total output error | E_TOT | I_P = ±5 A, T_A = 25°C | — | ±1.5 | — | % |

### 7.2 x20A variant (±20 A)

| Characteristic | Symbol | Test Conditions | Min | Typ | Max | Units |
|---|---|---|---|---|---|---|
| Optimized accuracy range | I_P | — | −20 | — | 20 | A |
| Sensitivity | Sens | Full I_P range, T_A = 25°C | 96 | 100 | 104 | mV/A |
| Noise | V_NOISE(PP) | Pk-pk, T_A=25°C, 100 mV/A, C_F=47 nF, C_OUT open, 2 kHz BW | — | 11 | — | mV |
| Zero-current output slope | ΔV_OUT(Q) | T_A = −40°C to 25°C | — | −0.34 | — | mV/°C |
| Zero-current output slope | ΔV_OUT(Q) | T_A = 25°C to 150°C | — | −0.07 | — | mV/°C |
| Sensitivity slope | ΔSens | T_A = −40°C to 25°C | — | 0.017 | — | mV/A/°C |
| Sensitivity slope | ΔSens | T_A = 25°C to 150°C | — | −0.004 | — | mV/A/°C |
| Total output error | E_TOT | I_P = ±20 A, T_A = 25°C | — | ±1.5 | — | % |

### 7.3 x30A variant (±30 A)

| Characteristic | Symbol | Test Conditions | Min | Typ | Max | Units |
|---|---|---|---|---|---|---|
| Optimized accuracy range | I_P | — | −30 | — | 30 | A |
| Sensitivity | Sens | Full I_P range, T_A = 25°C | 63 | 66 | 69 | mV/A |
| Noise | V_NOISE(PP) | Pk-pk, T_A=25°C, 66 mV/A, C_F=47 nF, C_OUT open, 2 kHz BW | — | 7 | — | mV |
| Zero-current output slope | ΔV_OUT(Q) | T_A = −40°C to 25°C | — | −0.35 | — | mV/°C |
| Zero-current output slope | ΔV_OUT(Q) | T_A = 25°C to 150°C | — | −0.08 | — | mV/°C |
| Sensitivity slope | ΔSens | T_A = −40°C to 25°C | — | 0.007 | — | mV/A/°C |
| Sensitivity slope | ΔSens | T_A = 25°C to 150°C | — | −0.002 | — | mV/A/°C |
| Total output error | E_TOT | I_P = ±30 A, T_A = 25°C | — | ±1.5 | — | % |

For all variants, total output error % is referenced to full-scale I_P and applies to the filtered output.

---

## 8. Dynamic Response — Filter Capacitor (C_F) Trade-offs

The FILTER pin (pin 6) sets device bandwidth via an internal 1.7 kΩ resistor forming an RC filter with an external capacitor C_F to ground. Larger C_F → lower bandwidth, lower noise, longer rise/power-on time.

| C_F (nF) | Rise time t_r (µs) |
|---|---|
| Open (no cap) | 3.5 |
| 1 | 5.8 |
| 4.7 | 17.5 |
| 22 | 73.5 |
| 47 | 88.2 |
| 100 | 291.3 |
| 220 | 623 |
| 470 | 1120 |

Bandwidth relation: f(−3 dB) = 0.35 / t_r

Power-on time (t_PO) also increases with C_F (approx. linear trend, higher at I_P = 5 A than I_P = 0 A per characterization curves).

Recommended default: C_F = 1 nF for general-purpose use (as shown in typical application circuit); increase C_F to trade bandwidth for lower noise in low-frequency sensing applications.

**Key design note — FILTER pin advantage:** Because the internal buffer amplifier sits after the internal filter resistor, adding C_F at the FILTER pin does NOT attenuate the DC/low-frequency output (unlike bolting an external RC filter directly onto VIOUT, which creates a resistive divider with the interface circuit's input impedance R_INTFC and causes temperature-dependent attenuation ΔV_ATT = V_IOUT × R_INTFC / (R_F + R_INTFC)).

---

## 9. Key Definitions

| Term | Definition |
|---|---|
| Sensitivity (Sens) | Change in output voltage per 1 A change in primary current; product of magnetic circuit sensitivity (G/A) and IC amplifier gain (mV/G), factory-trimmed for the device's full-scale current. |
| Noise (V_NOISE) | IC amplifier gain × Hall element noise floor (~1 G). Noise (mV) ÷ Sensitivity (mV/A) = smallest resolvable current. |
| Nonlinearity (E_LIN) | Deviation of output from a straight-line fit across full-scale current range, attributable to flux-concentrator saturation near full scale. |
| Symmetry (E_SYM) | Degree to which output magnitude is equal for positive vs. negative full-scale current. |
| Quiescent output voltage (V_IOUT(Q)) | Output at I_P = 0 A; nominally V_CC/2 (2.5 V at V_CC = 5 V). |
| Electrical offset voltage (V_OE) | Deviation of output from ideal V_CC/2 due to non-magnetic causes; divide by Sens to convert to amps. |
| Accuracy / Total output error (E_TOT) | Max deviation of actual output from ideal, evaluated across 4 conditions: 0 A @ 25°C, 0 A over ΔT, full-scale @ 25°C, full-scale over ΔT. |
| Ratiometry | V_IOUT(Q) and Sens scale proportionally with V_CC. ΔV_IOUT(Q)RAT (%) = 100 × (V_IOUT(Q)VCC / V_IOUT(Q)5V) / (V_CC/5V). ΔSens_RAT (%) analogous. |
| Power-on time (t_PO) | Time for output to settle within ±10% of steady-state after V_CC reaches minimum specified operating voltage, under an applied field. |
| Rise time (t_r) | Time from 10% to 90% of full-scale output in response to a step current input. Used to derive bandwidth: f(−3dB) = 0.35/t_r. Degraded by eddy current losses in the conductive IC ground plane. |
| Chopper stabilization | Modulation/demodulation technique separating DC offset from the magnetically-induced signal in the frequency domain, then low-pass filtering out the offset. Produces stable electrical offset voltage, immune to thermal/mechanical stress. |

---

## 10. Typical Application Circuits

| # | Application | Key Components | Notes |
|---|---|---|---|
| 1 | Basic current sensing | C_BYP = 0.1 µF (VCC), C_F = 1 nF (FILTER) | Standard reference circuit; VIOUT read directly |
| 2 | Peak detecting circuit | U1: LT1178 op-amp, D1: 1N914, Q1: 2N7002 reset switch, R1=1 MΩ, R2=33 kΩ, R3=330 kΩ, R4=10 kΩ, R_F=10 kΩ, C1/C2/C_OUT=0.1 µF | Captures V_PEAK; V_RESET line via Q1 resets peak hold |
| 3 | Gain-boost amplifier | LM321 op-amp, R1=R2=100 kΩ, R3=3.3 kΩ, R_F=1 kΩ, C_F=0.01 µF, C1=1000 pF | Increases gain to 610 mV/A (tested on ACS712ELC-05A) |
| 4 | Rectified output for A/D | D1: 1N4448W, R1=10 kΩ, R_F=2 kΩ, C_F=1 nF, C1=application-dependent | 3.3 V scaling + rectification; replaces current-transformer solutions; R1 omit for full range |
| 5 | 10 A overcurrent fault latch | LMV7235 comparator, D1: 1N914, R1=33 kΩ, R2=100 kΩ, R_PU=100 kΩ, C_F=1 nF | Fault threshold set by R1/R2 divider; latches until 5 V rail power-cycled; open-drain Fault output |

Note: the datasheet's Applications 6 and 7 (external RC filter vs. FILTER-pin RC filter, contrasted against the related ACS706 device) are not circuit examples for a specific function — their content is a design explanation and is captured in §8 above ("FILTER pin advantage").

---

## 11. Package Mechanical Summary — 8-pin SOIC (suffix LC)

Reference: Allegro DWG-0000385 Rev. 2 / JEDEC MS-012AA. Dimensions in mm, not to scale.

| Dimension | Value (mm) |
|---|---|
| Body length | 4.90 BSC |
| Body width | 3.90 BSC |
| Overall length (incl. leads) | 6.00 BSC |
| Lead pitch | 1.27 BSC |
| Standoff height | 0.15 (+0.10/−0.05) |
| Seating height | 1.62 (+0.13/−0.27) |
| Lead width | 0.41 ± 0.10 |
| Lead foot length | 0.84 (+0.43/−0.44) |
| Gauge plane offset | 0.25 BSC |
| Foot angle | 0°–8° |
| PCB pad pitch (land pattern) | 1.27, 1.75 offset, 5.60 span (ref. IPC7351 SOIC127P600X175-8M) |

Branding: Line 1 = part number (8 chars), Line 2 = temp/package/amps (8 chars), Line 3 = first 5 chars of lot number (5 chars); belly brand = country of origin + lot number.

---

## 12. Revision History (of this datasheet)

| Rev | Date | Description |
|---|---|---|
| 15 | 2012-11-16 | Updated rise time and isolation, I_OUT reference data, patents |
| 16 | 2017-06-05 | Updated product status |
| 17 | 2018-12-10 | Updated certificate numbers |
| 18 | 2019-05-17 | Updated TUV certificate mark, minor editorial updates |
| 19 | 2020-01-30 | Updated product status, minor editorial updates |
| 20 | 2022-02-07 | Updated package drawing |
| 21 | 2023-02-09 | Updated selection guide heading |
| 22 | 2024-02-13 | Updated Selection Guide and Isolation Characteristics; removed patent information |

---

## 13. Compliance & Usage Restrictions

- Certifications: TÜV America (Cert. U8V 15 05 54214 038, CB 13 06 54214 026).
- Copyright 2024, Allegro MicroSystems.
- **Not to be used in life-support devices or systems** where failure could reasonably cause bodily harm.
- **Not intended for automotive applications.**
- Source datasheet is an uncontrolled document once copied; verify current revision at www.allegromicro.com before design finalization.
