# DRV8412 / DRV8432 — Dual Full-Bridge PWM Motor Driver

**Manufacturer:** Texas Instruments
**Document:** SLES242G — December 2009, Revised December 2014
**Devices covered:** DRV8412, DRV8432

> This file is a structured, machine-readable conversion of the TI DRV8412/DRV8432 datasheet, reorganized for quick lookup and reasoning by an AI agent (e.g., for firmware/driver development, schematic review, or BOM selection). All electrical values, pin mappings, and formulas are preserved verbatim from the source. Figures/schematics/waveforms from the original PDF are described in text form; refer to the original PDF for the actual images.

---

## 1. Device Summary

| | DRV8412 | DRV8432 |
|---|---|---|
| Package | HTSSOP (44-pin), "Power Pad Down" | HSSOP (36-pin), "Power Pad Up" |
| Body size (nom) | 14.00 mm × 6.10 mm | 15.90 mm × 11.00 mm |
| Continuous output current (dual full-bridge) | 2 × 3 A (2 × 6 A peak) | 2 × 7 A (2 × 12 A peak) |
| Continuous output current (parallel mode) | 6 A (12 A peak) | 14 A (24 A peak) |
| Package drawing / orderable | DDW | DKD |

**Key features:**
- High-efficiency power stage, up to 97% efficiency; low R_DS(on) MOSFETs, 110 mΩ typ at T_J = 25°C
- Operating supply voltage up to 52 V (PVDD)
- PWM operating frequency up to 500 kHz
- Integrated self-protection: undervoltage, overtemperature (2-stage), overload, short-circuit
- Programmable cycle-by-cycle current limit protection
- Independent supply and ground pins for each half-bridge (enables per-bridge current sensing via shunt resistor, and mixed-voltage multi-motor operation)
- Intelligent gate drive with cross-conduction prevention
- No external snubber or Schottky diode required

**Two power supply rails required:**
- 12 V nominal for GVDD (gate drive) and VDD (digital regulator)
- Up to 50 V (52.5 V max) for PVDD (motor supply)

**Applications:** Brushed DC and stepper motors, three-phase PMSM, robotic/haptic control, actuators/pumps, precision instruments, TEC drivers, LED lighting drivers.

**Orderable parts:**

| Orderable Device | Status | Package | Pins | Qty/Reel | Op Temp | Marking |
|---|---|---|---|---|---|---|
| DRV8412DDW | Active | HTSSOP (DDW) | 44 | 35 (tray) | −40 to 85°C | DRV8412 |
| DRV8412DDWR | Active | HTSSOP (DDW) | 44 | 2000 (reel) | −40 to 85°C | DRV8412 |
| DRV8432DKD | Active | HSSOP (DKD) | 36 | 29 (tray) | −40 to 85°C | DRV8432 |
| DRV8432DKDR | Active | HSSOP (DKD) | 36 | 500 (reel) | −40 to 85°C | DRV8432 |

All parts: Green (RoHS & no Sb/Br), CU NIPDAU lead/ball finish.
MSL: DRV8412 = Level-3-260C-168HR; DRV8432 = Level-4-260C-72HR.

---

## 2. Pin Configuration and Functions

### 2.1 DRV8412 (44-pin DDW, top view)

| Pin # | Name | Pin # | Name |
|---|---|---|---|
| 1 | GVDD_C | 23 | GVDD_A |
| 2 | VDD | 24 | BST_A |
| 3 | NC | 25 | NC |
| 4 | NC | 26 | PVDD_A |
| 5 | PWM_D | 27 | PVDD_A |
| 6 | RESET_CD | 28 | OUT_A |
| 7 | PWM_C | 29 | GND_A |
| 8 | M1 | 30 | GND_B |
| 9 | M2 | 31 | OUT_B |
| 10 | M3 | 32 | PVDD_B |
| 11 | VREG | 33 | BST_B |
| 12 | AGND | 34 | BST_C |
| 13 | GND | 35 | PVDD_C |
| 14 | OC_ADJ | 36 | OUT_C |
| 15 | PWM_B | 37 | GND_C |
| 16 | RESET_AB | 38 | GND_D |
| 17 | PWM_A | 39 | OUT_D |
| 18 | FAULT (active-low) | 40 | PVDD_D |
| 19 | NC | 41 | PVDD_D |
| 20 | NC | 42 | NC |
| 21 | OTW (active-low) | 43 | BST_D |
| 22 | GVDD_B | 44 | GVDD_D |

Exposed Thermal Pad on bottom — solder to PCB landing pad, connect to GND via thermal vias.

### 2.2 DRV8432 (36-pin DKD, top view)

| Pin # | Name | Pin # | Name |
|---|---|---|---|
| 1 | GVDD_B | 19 | GVDD_D |
| 2 | OTW (active-low) | 20 | BST_D |
| 3 | FAULT (active-low) | 21 | PVDD_D |
| 4 | PWM_A | 22 | OUT_D |
| 5 | RESET_AB | 23 | GND_D |
| 6 | PWM_B | 24 | GND_C |
| 7 | OC_ADJ | 25 | OUT_C |
| 8 | GND | 26 | PVDD_C |
| 9 | AGND | 27 | BST_C |
| 10 | VREG | 28 | BST_B |
| 11 | M3 | 29 | PVDD_B |
| 12 | M2 | 30 | OUT_B |
| 13 | M1 | 31 | GND_B |
| 14 | PWM_C | 32 | GND_A |
| 15 | RESET_CD | 33 | OUT_A |
| 16 | PWM_D | 34 | PVDD_A |
| 17 | VDD | 35 | BST_A |
| 18 | GVDD_C | 36 | GVDD_A |

Exposed Heat Slug on bottom — mount heat sink with thermal interface material for best thermal performance (no direct PCB solder connection required, "N/A" for thermal pad row).

### 2.3 Pin Function Descriptions (applies to both parts; pin # differs — see tables above)

| Name | I/O Type | Description |
|---|---|---|
| AGND | P | Analog ground |
| BST_A / BST_B / BST_C / BST_D | P | High-side bootstrap supply; external capacitor to OUT_x required |
| GND | P | Ground |
| GND_A / GND_B / GND_C / GND_D | P | Power ground for half-bridge A/B/C/D |
| GVDD_A / GVDD_B / GVDD_C / GVDD_D | P | Gate-drive voltage supply |
| M1 | I | Mode selection pin |
| M2 | I | Mode selection pin |
| M3 | I | Reserved mode selection pin; AGND connection recommended |
| NC | — | No connection; ground connection recommended |
| OC_ADJ | O | Analog overcurrent programming pin; requires resistor to AGND |
| OTW (active-low, open-drain) | O | Overtemperature warning. Internal pullup to VREG (3.3 V). Add external pullup to 5 V for 5-V logic compliance |
| OUT_A / OUT_B / OUT_C / OUT_D | O | Half-bridge output |
| PVDD_A / PVDD_B / PVDD_C / PVDD_D | P | Power supply input per half-bridge; requires close decoupling capacitor to ground |
| PWM_A / PWM_B / PWM_C / PWM_D | I | Input signal per half-bridge |
| RESET_AB | I | Reset for half-bridge A & B, active-low |
| RESET_CD | I | Reset for half-bridge C & D, active-low |
| FAULT (active-low, open-drain) | O | Fault signal. Internal pullup to VREG (3.3 V); external pullup to 5 V for 5-V logic compliance |
| VDD | P | Digital regulator supply; requires decoupling capacitor to ground |
| VREG | P | Digital regulator filter pin; requires 0.1 µF capacitor to AGND |
| THERMAL PAD (DRV8412 only) | T | Solder to PCB landing pad; connect to bottom-side ground plane via thermal vias; connect to GND |
| HEAT SLUG (DRV8432 only) | T | Mount heat sink w/ thermal interface on top for best thermal performance |

### 2.4 Mode Selection Pins (M3, M2, M1)

| M3 | M2 | M1 | Output Configuration | Description |
|---|---|---|---|---|
| 0 | 0 | 0 | 2 FB or 4 HB | Dual full bridges (2 PWM inputs each) or 4 half bridges, **with cycle-by-cycle current limit** |
| 0 | 0 | 1 | 2 FB or 4 HB | Dual full bridges or 4 half bridges, **with OC latching shutdown (no CBC current limit)** |
| 0 | 1 | 0 | 1 PFB | Parallel full bridge, with cycle-by-cycle current limit |
| 0 | 1 | 1 | 2 FB | Dual full bridges (1 PWM input each, complementary PWM on 2nd half-bridge), with CBC current limit |
| 1 | x | x | Reserved | — |

---

## 3. Electrical Specifications

### 3.1 Absolute Maximum Ratings (over operating free-air temp range unless noted)

| Parameter | Min | Max | Unit |
|---|---|---|---|
| VDD to GND | −0.3 | 13.2 | V |
| GVDD_x to GND | −0.3 | 13.2 | V |
| PVDD_x to GND_x (transient spike max; DC max is lower) | −0.3 | 70 | V |
| OUT_x to GND_x (transient spike max) | −0.3 | 70 | V |
| BST_x to GND_x (transient spike max) | −0.3 | 80 | V |
| Transient peak output current per pin (OC-protection-limited pulse width) | | 16 | A |
| Transient peak output current for latch shutdown (per pin) | | 20 | A |
| VREG to AGND | −0.3 | 4.2 | V |
| GND_x to GND | −0.3 | 0.3 | V |
| GND to AGND | −0.3 | 0.3 | V |
| PWM_x to GND | −0.3 | VREG + 0.5 | V |
| OC_ADJ, M1, M2, M3 to AGND | −0.3 | 4.2 | V |
| RESET_x, FAULT, OTW to GND | −0.3 | 7 | V |
| Continuous sink current (FAULT, OTW) | | 9 | mA |
| Operating junction temperature T_J | −40 | 150 | °C |
| Storage temperature T_stg | −55 | 150 | °C |

Note: JEDEC JEP157 states 250-V CDM allows safe manufacturing with standard ESD control process.

### 3.2 ESD Ratings

| Parameter | Value | Unit |
|---|---|---|
| V(ESD) — Charged device model (CDM), JESD22-C101, all pins | ±1500 | V |

> **Handling caution:** These devices have limited built-in ESD protection. Leads should be shorted together or the device placed in conductive foam during storage or handling to prevent electrostatic damage to the MOS gates.

### 3.3 Recommended Operating Conditions

| Parameter | Description | Min | Nom | Max | Unit |
|---|---|---|---|---|---|
| PVDD_X | Half-bridge X (A,B,C,D) DC supply voltage | 0 | 50 | 52.5 | V |
| GVDD_X | Supply for logic regulators/gate-drive circuitry | 10.8 | 12 | 13.2 | V |
| VDD | Digital regulator supply voltage | 10.8 | 12 | 13.2 | V |
| I_O_PULSE | Pulsed peak current per output pin (thermally limited) | | | 15 | A |
| I_O | Continuous current per output pin (DRV8432) | | | 9 | mA *(sic — see note below)* |
| F_SW | PWM switching frequency | | | 500 | kHz |
| R_OCP_CBC | OC programming resistor range, cycle-by-cycle mode | 24 | | 200 | kΩ |
| R_OCP_OCL | OC programming resistor range, OC latching shutdown mode | 22 | | 200 | kΩ |
| C_BST | Bootstrap capacitor range | 33 | | 220 | nF |
| t_ON_MIN | Min PWM pulse duration, low side, for charging bootstrap capacitor | | 50 | | ns |
| T_A | Operating ambient temperature | −40 | | 85 | °C |

> Note: The "9 mA" continuous-current row appears exactly as printed in the source datasheet table; this is very likely a typo/table artifact in the original TI document (continuous output current is specified elsewhere as several amps — see Section 1 summary table). Treat the amp-level continuous current ratings in Section 1 as authoritative for output current capability.

### 3.4 Thermal Information

| Metric | DRV8412 (DDW, 44-pin) | DRV8432 (DKD, 36-pin) | Unit |
|---|---|---|---|
| R_θJA — junction-to-ambient | 24.5 | 13.3 (with heat sink) | °C/W |
| R_θJC(top) — junction-to-case (top) | 7.8 | 0.4 | °C/W |
| R_θJB — junction-to-board | 5.5 | 13.3 | °C/W |
| ψ_JT | 0.1 | 0.4 | °C/W |
| ψ_JB | 5.4 | 13.3 | °C/W |
| R_θJC(bot) — junction-to-case (bottom) | 0.2 | N/A | °C/W |

### 3.5 Package Heat Dissipation Ratings

| Parameter | DRV8412 | DRV8432 |
|---|---|---|
| R_θJC, junction-to-case (power pad / heat slug) | 1.1 °C/W | 0.9 °C/W |
| R_θJA, junction-to-ambient | 25 °C/W | Not specified — device not intended for use without heatsink |
| Exposed power pad / heat slug area | 34 mm² | 80 mm² |

### 3.6 Package Power Deratings — DRV8412 (44-pin TSSOP/DDW), based on EVM board layout

| T_A = 25°C Power Rating | Derating factor above 25°C | T_A = 70°C | T_A = 85°C | T_A = 125°C |
|---|---|---|---|---|
| 5.0 W | 40.0 mW/°C | 3.2 W | 2.6 W | 1.0 W |

### 3.7 Electrical Characteristics

Conditions: T_A = 25°C, PVDD = 50 V, GVDD = VDD = 12 V, f_SW = 400 kHz, unless otherwise noted.

**Internal voltage regulator and current consumption**

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| V_REG | VDD = 12 V (reference node only) | 2.95 | 3.3 | 3.65 | V |
| I_VDD | Idle/reset mode | | 9 | 12 | mA |
| I_VDD | Operating, 50% duty cycle | | 10.5 | | mA |
| I_GVDD_X | Reset mode | | 1.7 | 2.5 | mA |
| I_GVDD_X | Operating, 50% duty cycle | | 8 | | mA |
| I_PVDD_X | Reset mode (per half-bridge idle current) | | 0.7 | 1 | mA |

**Output stage**

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| R_DS(on), low side | T_J=25°C, GVDD=12V, incl. metallization bond wire & pin resistance | | | 110 | mΩ |
| R_DS(on), high side | same | | | 110 | mΩ |
| V_F — diode forward voltage drop | T_J=25–125°C, I_O=5A | | | 1 | V |
| t_R — output rise time | Resistive load, I_O=5A | | | 14 | ns |
| t_F — output fall time | Resistive load, I_O=5A | | | 14 | ns |
| t_PD_ON — propagation delay, FET on | Resistive load, I_O=5A | | | 38 | ns |
| t_PD_OFF — propagation delay, FET off | Resistive load, I_O=5A | | | 38 | ns |
| t_DT — dead time HS/LS | Resistive load, I_O=5A | | | 5.5 | ns |

**I/O protection**

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| V_uvp,G — GVDD_X undervoltage protection threshold | | | | 8.5 | V |
| V_uvp,hyst — hysteresis for gate supply UV event (by design) | | | 0.8 | | V |
| OTW — overtemperature warning (by design) | | 115 | 125 | 135 | °C |
| OTW_hyst — hysteresis to reset OTW (by design) | | | 25 | | °C |
| OTSD — overtemperature shutdown (by design) | | | 150 | | °C |
| OTE-OTW differential (by design) | | | 25 | | °C |
| OTSD_HYST — hysteresis for FAULT release after OTSD (by design) | | | 25 | | °C |
| I_OC — overcurrent limit protection | R_OCP = 27 kΩ, programmable | | 9.7 | | A |
| I_OCT — overcurrent response time | Short condition → Hi-Z of affected FET(s) | | 250 | | ns |
| R_PD — internal pulldown at half-bridge output | Active when RESET_AB/RESET_CD asserted (bootstrap charge) | | 1 | | kΩ |

**Static digital specifications**

| Parameter | Condition | Min | Max | Unit |
|---|---|---|---|---|
| V_IH | PWM_A/B/C/D, M1, M2, M3 | 2 | 3.6 | V |
| V_IH | RESET_AB, RESET_CD | 2 | 5.5 | V |
| V_IL | PWM_A/B/C/D, M1, M2, M3, RESET_AB, RESET_CD | | 0.8 | V |
| I_lkg — input leakage current | | −100 | 100 | µA |

**OTW / FAULT**

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| R_INT_PU — internal pullup, OTW/FAULT to VREG | | 20 | 26 | 35 | kΩ |
| V_OH | Internal pullup resistor only | 2.95 | 3.3 | 3.65 | V |
| V_OH | External pullup of 4.7 kΩ to 5 V | 4.5 | | 5 | V |
| V_OL | I_O = 4 mA | | 0.2 | 0.4 | V |

### 3.8 Typical Characteristics (Figures 1–5, qualitative summary)

> These describe the shape/trend of the curves plotted in the original PDF (Section 6.8). Exact digitized data points aren't recoverable from the source PDF's text layer — treat this as a qualitative guide and consult the original graphs for precise values at a specific operating point.

| Figure | Title | Conditions | Trend described |
|---|---|---|---|
| 1 | Efficiency vs Switching Frequency (DRV8432) | Full-bridge load = 5 A, PVDD = 50 V, T_C = 75°C | Efficiency stays high (roughly 90–97%) across the 0–500 kHz sweep, with a gradual downward taper as switching frequency increases toward 500 kHz (higher switching losses at higher frequency). |
| 2 | Normalized R_DS(on) vs Gate Drive | T_J = 25°C | Normalized R_DS(on)/(R_DS(on) at 12V) decreases roughly monotonically from ~1.08 at 8.0 V gate drive down to ~1.00 at 12 V — i.e., R_DS(on) is higher at lower gate-drive voltage. |
| 3 | Normalized R_DS(on) vs Junction Temperature | GVDD = 12 V | Normalized R_DS(on)/(R_DS(on) at 25°C) rises roughly linearly with temperature, from ~0.7 at −40°C to ~1.5 near 125–140°C — a standard positive temperature coefficient for power MOSFET R_DS(on). |
| 4 | Drain-to-Source Diode Forward-On Characteristics | T_J = 25°C | Classic diode turn-on shape: current stays near zero until roughly 0.6–0.7 V forward voltage, then rises steeply, reaching ~5 A around 1.0 V. |
| 5 | Output Duty Cycle vs Input Duty Cycle | f_S = 500 kHz, T_C = 25°C | Essentially linear 1:1 tracking across the full 0–100% range — confirms good PWM duty-cycle fidelity through the signal path at max rated switching frequency. |

Additionally, Section 8.2.1.3 of the source ("Application Curves," Figures 9–14) contains oscilloscope captures validating: brushed-DC driving, stepper full-stepping at 24 V and 12 V, stepper half-stepping at 12 V, 128-microstepping at 12 V, and PWM_A-to-OUT_A propagation timing. These are scope-capture illustrations, not tabulated data — refer to the original PDF pages for the actual traces.

### 3.9 Overcurrent Programming-Resistor Table

R_OC_ADJ vs. typical OC threshold (±10% device-to-device tolerance typical; feature is for system protection, not precise current control):

| OC_ADJ Resistor (kΩ) | Max current before OC occurs (A) |
|---|---|
| 22 (recommended for OC Latching mode only) | 11.6 |
| 24 | 10.7 |
| 27 | 9.7 |
| 30 | 8.8 |
| 36 | 7.4 |
| 39 | 6.9 |
| 43 | 6.3 |
| 47 | 5.8 |
| 56 | 4.9 |
| 68 | 4.1 |
| 82 | 3.4 |
| 100 | 2.8 |
| 120 | 2.4 |
| 150 | 1.9 |
| 200 | 1.4 |

---

## 4. Functional Description

### 4.1 Overview

The DRV841x2 is a dual full-bridge motor driver with an advanced protection system. Low R_DS(on) MOSFETs and intelligent gate drive give up to 97% efficiency, enabling smaller power supplies/heatsinks.

### 4.2 Functional Block Diagram (textual)

- Shared logic block: **Protection and I/O Logic**, fed by:
  - Undervoltage Protection block (monitors VDD, GVDD_A/B/C/D — 4 lines in/out)
  - Power-On Reset block
  - Temp. Sense block
  - Overload Protection block (fed by I_sense, which reads OC_ADJ)
  - I/O pins into this block: OTW (out, via internal pullup to VREG), FAULT (out, via internal pullup to VREG), M1/M2/M3 (in), RESET_AB (in), RESET_CD (in)
- VREG block: powered from VDD, referenced to AGND, output to VREG pin
- Four identical half-bridge channels (D, C, B, A), each with: **PWM Rcv. → Ctrl. → Timing → Gate Drive → half-bridge power stage**
  - Each channel has its own GVDD_x, BST_x (with internal diode to GVDD_x), PVDD_x, OUT_x, GND_x pins
  - Each channel has an "FB/PFB-Configuration Pulldown Resistor" at its output
  - PWM_D/PWM_C feed a shared RESET_CD line into their Ctrl. blocks; PWM_B/PWM_A feed a shared RESET_AB line into their Ctrl. blocks

### 4.3 Error Reporting (FAULT / OTW)

- FAULT and OTW are both **active-low, open-drain** outputs for protection-mode signaling to the system controller.
- FAULT goes low on: overtemperature shutdown, overcurrent shutdown, or undervoltage protection.
- OTW goes low when junction temperature exceeds 125°C.
- Internal pullup resistor to VREG (3.3 V) provided on both; add external pullup to 5 V for 5-V logic compliance.
- TI recommends monitoring OTW and reducing load current in response, to avoid reaching OTSD.

**Table 1 — Protection Mode Signal Decode**

| FAULT | OTW | Description |
|---|---|---|
| 0 | 0 | Overtemperature warning AND (OT shutdown OR OC shutdown OR undervoltage protection) occurred |
| 0 | 1 | Overcurrent shutdown OR GVDD undervoltage protection occurred |
| 1 | 0 | Overtemperature warning only |
| 1 | 1 | Device under normal operation |

### 4.4 Device Protection System (general)

On any fault, the device immediately sets the affected half-bridge outputs to Hi-Z and asserts FAULT low. For non-OC/non-OT faults, the device auto-recovers when the condition clears or GVDD increases. **For highest reliability, wait ≥1 second after shutdown before externally resetting** when recovering from an OCSD or OTSD fault.

### 4.5 Bootstrap Capacitor Undervoltage Protection (BST_UVP)

- At low switching frequency (e.g., <10 kHz with a 100 nF bootstrap cap), bootstrap voltage may sag below the level needed for the high-side driver.
- BST_UVP circuit initiates recharge sequences (briefly turns off the high-side FET) until the bootstrap cap is properly charged.
- Can also trigger if PWM duty cycle is too high (e.g., <20 ns low-side off-time at 10 kHz).
- Bootstrap cap may not charge under no-load/extremely-light-load — recommended to keep the low-side FET on for ≥50 ns per PWM cycle to avoid BST_UVP.
- For switching frequency <10 kHz, use a larger bootstrap cap (e.g., 1 µF at 800 Hz). If bootstrap cap > 220 nF, add 5-Ω resistors between the 12-V GVDD supply and each GVDD_X pin to limit inrush current.

### 4.6 Overcurrent (OC) Protection

Independent, fast-reacting current detectors with a programmable trip threshold on all high-side and low-side FETs. Two modes, selected via mode pins:

**Cycle-by-cycle (CBC) current limiting mode:**
- Two protection layers: (1) CBC current limiting — prevents further current increase without shutting down (handles motor-startup/transient inrush); (2) a second layer triggers latching shutdown (Hi-Z) if current can't be controlled (e.g., short-to-power/short-to-ground).
- Current limiting and OC protection are independent per half-bridge (A, B, C, D).
- Low-side FET OC detected → device turns off affected low-side FET, keeps high-side FET off at that half-bridge until next PWM cycle.
- High-side FET OC detected → device turns off affected high-side FET, turns on the low-side FET at that half-bridge until next PWM cycle.
- **Important:** if a half-bridge input is held at a constant (DC) logic level when a CBC OC event occurs, that half-bridge stays Hi-Z after the event ends. Cycling PWM_x (i.e., toggling the input) is required to resume normal operation — CBC mode does not support a half-bridge PWM input tied to a constant DC level.

**OC latching shutdown (OCL) mode:**
- CBC current limit and auto error-recovery are disabled.
- Any OC condition → immediate shutdown.
- Requires RESET_AB and/or RESET_CD to be asserted (low→high transition) to restore normal operation after the OC condition is removed.

**OC threshold programming:**
- Single external resistor from OC_ADJ pin to GND (actually to AGND per pin table / layout guidance — see Section 6.4). See table in Section 3.9.
- ~20% device-to-device variation in OC threshold is possible even with fixed R_OC_ADJ — designed for system protection, not precise current control.
- A properly functioning OC detector assumes an inductor or power ferrite bead is present at the power-stage output. **Short-circuit protection is NOT guaranteed with a direct short at the output pins.**

**Inductor / ferrite bead guidance for short-circuit protection:**
- Normal operation: motor inductance >10 µH is typically sufficient for low di/dt and proper CBC protection — no extra output inductor usually needed.
- Short condition: load inductance disappears, current can rise fast enough (high di/dt, low impedance) to exceed absolute max ratings before OC detection engages (250 ns response time). A ferrite bead or inductor at the output is recommended to slow the current rise.
- Ferrite bead sizing: current rating > system RMS current; avoid beads designed for very high frequency; recommend ≥10 Ω impedance at 10 MHz or lower to effectively limit current rise rate during short circuit.
- Tested example: TDK MPZ2012S300A (0805 size) validated in DRV8412 systems; other beads with similar frequency characteristics usable. Higher-current DRV8432 applications may need an inductor instead if no suitable bead is found.

**Minimum inductor value formula:**

```
Loc_min = (PVDD × Toc_delay) / (Ipeak − Iave)

where:
  Toc_delay = 250 ns
  Ipeak     = 15 A (must stay below absolute max rating)
```

Because inductors saturate past their rated current, use one with a doubled value or a rating well above the operating condition.

### 4.7 Overtemperature Protection

Two-level system:
- **OTW** (warning): active-low, asserted when T_J exceeds 125°C (nominal).
- **OTSD** (shutdown): if T_J exceeds 150°C (nominal), all half-bridge outputs go Hi-Z and FAULT asserts low. **OTSD is latched** — RESET_AB and RESET_CD must both be asserted low to clear the latch.

### 4.8 Undervoltage Protection (UVP) and Power-On Reset (POR)

- POR resets the overcurrent circuit and ensures all circuits are operational once GVDD_X and VDD reach 9.8 V (typical) during power-up.
- GVDD_X and VDD are independently monitored; a supply drop below the UVP threshold on ANY VDD or GVDD_X pin → all half-bridge outputs immediately go Hi-Z and FAULT asserts low.
- Device auto-resumes once bootstrap-capacitor supply voltages rise back above the UVP threshold.

### 4.9 Device Reset (RESET_AB / RESET_CD)

- RESET_AB low → forces all 4 power-stage FETs in half-bridges A & B to Hi-Z.
- RESET_CD low → forces all 4 power-stage FETs in half-bridges C & D to Hi-Z.
- Asserting either reset low also enables a weak pulldown on the corresponding half-bridge outputs (accommodates bootstrap charging before switching starts).
- A rising edge (low→high) on RESET_AB clears an OC-shutdown fault + FAULT signal for half-bridges A/B; same for RESET_CD → C/D.
- **For an OTSD event, BOTH RESET_AB and RESET_CD must transition low→high** to clear the fault and FAULT signal.

---

## 5. Device Functional Modes

Four modes of operation, selected by M1/M2/M3 (see Section 2.4 table):

1. **Dual full bridges (FB, 2 PWM inputs each) / 4 half bridges (HB)** — with CBC current limit
2. **Dual full bridges / 4 half bridges** — with OC latching shutdown (no CBC current limit)
3. **Parallel full bridge (PFB)** — with CBC current limit
4. **Dual full bridges (1 PWM input each, complementary)** — with CBC current limit

**Mode 1 & 2:** PWM_A controls half-bridge A, PWM_B controls B, PWM_C controls C, PWM_D controls D independently.

**Mode 3 (parallel full bridge):** PWM_A controls both half-bridges A and B; PWM_B controls both half-bridges C and D. PWM_C and PWM_D are unused (tie to ground). Bridges A&B are synchronized internally (even during CBC), as are C&D. **OUT_A and OUT_B must be connected together, and OUT_C and OUT_D must be connected together, after the output inductor or ferrite bead.** If RESET_AB or RESET_CD is low, all four outputs go Hi-Z.

**Mode 4:** One PWM signal drives a full bridge to save MCU I/O — PWM_A controls half-bridges A & B, PWM_C controls half-bridges C & D. Half-bridge B operation is complementary to A; D is complementary to C. Example: PWM_A high → high-side FET(A) + low-side FET(B) on; low-side FET(A) + high-side FET(B) off. PWM_B and PWM_D are unused in this mode (tie to ground).

**CBC modes (1, 2, 4) — important restriction:** once the CBC current limit is hit, the driver is deactivated until the next PWM cycle. Recovery requires the PWM input for that driver to be toggled — **CBC mode does not support operation with a half-bridge PWM input tied to a constant DC logic level.**

**Current sensing:** Because each half-bridge has independent supply/ground pins, a shunt resistor can be placed between PVDD and PVDD_x (high-side sensing, recommended for differential sensing) or between GND_x and GND (low-side sensing). If low-side sensing is used: shunt resistor ≤10 mΩ or sense voltage ≤100 mV recommended (high bias voltage on low-side sensing can affect device operation).

---

## 6. Application & Design Guidance

> Per TI's standard disclaimer: application-section information is not part of the component specification; customers must validate/test their own implementation.

### 6.1 General Application Notes

- Typical use: drive 2 brushed DC motors, or 1 stepper motor.
- Also usable for 3-phase PMSM / sinewave BLDC, TEC drivers, and general DC/resistive-load output (LC filter recommended for non-motor loads).

### 6.2 Typical Application Topologies (see original PDF for full schematics)

1. **Full Bridge Mode** — independent PWM_A/B/C/D drive 2 full H-bridges; optional Rsense_AB / Rsense_CD shunts; controller options mentioned: MSP430, C2000, or Stellaris MCU.
2. **Parallel Full Bridge Mode** — PWM_A drives OUT_A+OUT_B in parallel (via Loc inductors), PWM_B drives OUT_C+OUT_D in parallel; optional Rsense shunts.
3. **Stepper Motor Operation** — standard 2-phase stepper driven from full-bridge outputs.
4. **TEC Driver** — LC-filtered (4.7 µH + 47 µF per leg) DC output to a thermoelectric cooler; same topology usable for general DC output.
5. **LED Lighting Driver** — DRV8412 driving multiple LED strings with current-sense amps (AMP blocks) feeding back to MCU ADC; PWM from MCU; up to 50 V input, 12 V logic supply.

### 6.3 Design Parameters — Worked Example (Section 8.2.1.1 of source)

| Design Parameter | Reference | Example Value |
|---|---|---|
| Motor voltage | PVDD_x | 24 V |
| Motor current (peak and RMS) | I_PVDD | 6-A peak, 3-A RMS |
| Overcurrent threshold | OC_TH | OC_ADJ = 27 kΩ → 9.7 A |
| Bridge mode | M1M2 | Parallel full bridge |

### 6.4 Detailed Design Procedure Notes

- **Motor voltage:** Higher voltage → faster current change through inductive windings → higher RPM. Lower voltage → more accurate phase-current control.
- **12-V supply current requirement:** Total supply current is low at room temp (<50 mA) but can rise significantly at high device temperature (e.g., >125°C) under heavy load, due to substrate current collection by 12-V guard rings. Recommend 12-V supply current capability ≥5–10% of load current, and no less than 100 mA, across all temperatures.
- **Decoupling capacitor voltage rating:** Consider temperature, ripple current, voltage overshoot. Use ceramic X5R-or-better for high-frequency decoupling. For a 50-V application, recommend ≥63 V rated capacitors.
- **Overcurrent threshold selection:** Consider peak current allowed under normal behavior, resistor tolerance, and the ±10% (nominal, per Table in 3.9) tolerance on OC threshold. Example: if 6 A is the highest normal system current, a 27-kΩ OC_ADJ resistor (10% tolerance) is reasonable, setting OC_TH to ~8–12 A.
- **Sense resistor selection:** Must be surface-mount, low-inductance, rated for sufficient power, placed close to the driver. Power dissipation = I_RMS² × R (e.g., 2 A RMS through 0.05 Ω → 0.2 W). Consider PCB thermal margin; measure actual resistor temperature in the final system. For power resistors, consider multiple standard resistors in parallel to distribute current/heat.

### 6.5 Power Supply Recommendations

**Bulk capacitance:** More local bulk capacitance is generally better (tradeoff: cost, size). Sizing depends on: max motor system current, power supply's own capacitance/current-sourcing ability, parasitic inductance between supply and motor system, acceptable voltage ripple, motor type (brushed DC / BLDC / stepper), and motor braking method. Insufficient local bulk capacitance → voltage changes under current transients; adequate capacitance → stable voltage, fast high-current delivery. **System-level testing required to determine correct bulk capacitor size.** Bulk capacitor voltage rating should exceed operating voltage (margin for motor regenerative energy transfer back to the supply).

**Power supplies:** Only a 12-V supply is needed in addition to PVDD. Internal regulator supplies digital/low-voltage analog circuitry. High-side gate drive uses a floating supply via built-in bootstrap circuitry + external bootstrap capacitor. Each half-bridge has independent GVDD_x, BST_x, PVDD_x pins, plus a shared VDD pin for common circuits. Decoupling capacitors should be placed as close as possible to their pins, with minimal inductance and a short ground return path.

**Bootstrap capacitor:** Small ceramic capacitor (X5R or better) from each BST_x to OUT_x. Charged through internal diode from GVDD_x when output is low; bootstrap potential shifts above output when output is high, supplying the high-side gate driver. For 10 kHz–500 kHz switching: 100 nF ceramic (X5R+, 0603/0805) recommended. Below 10 kHz switching: increase bootstrap cap value.

**PVDD decoupling:** Each PVDD_x pin needs its own ceramic (X5R+) decoupling capacitor placed as close as possible to the pin — follow the DRV841x2 EVM PCB layout as reference.

**Supply quality:** 12-V supply should be low-noise, low-output-impedance. 50-V power stage supply assumed low-impedance, low-noise. Power-supply sequencing is NOT critical (internal POR handles it); voltage ramp rates (dv/dt) are non-critical within the specified range, because the device is protected against erroneous power-stage turn-on from parasitic gate charging.

**Power-up:** No specific power-up sequence required. Outputs stay Hi-Z until GVDD_X and VDD exceed the UVP threshold. Recommended (not required) to hold RESET_AB and RESET_CD low during power-up, to enable weak pulldown / bootstrap charging.

**Power-down:** No specific power-down sequence required. Device remains fully operational as long as GVDD_X and VDD stay above UVP threshold. Recommended (not required) to hold RESET_AB and RESET_CD low during power-down.

### 6.6 System Design Recommendations

- **VREG pin:** Internal-logic use only — do NOT use as an external voltage source.
- **VDD pin:** Transient current can be much higher than average current. Use a low-resistance path to GVDD. Place a 22–47 µF capacitor on VDD alongside the 100 nF–1 µF decoupling cap, for transient voltage stability.
- **OTW pin:** Use with the MCU to reduce system power when OTW goes low, to prevent reaching OTSD at a higher temperature.
- **Mode select pins (M1/M2/M3):** Connect to VREG (logic high) or AGND (logic low). Do NOT connect mode pins directly to board GND if a 1-Ω resistor is used between AGND and GND.
- **Parallel mode operation:** Requires a minimum of 30–100 nH inductance (or a ferrite bead) after the output pins (e.g., between OUT_A and OUT_B) before joining the two channels, to prevent shoot-through from paralleled-channel mismatch (process variation, unsymmetrical PCB layout, etc.).
- **TEC driver / non-motor loads:** Use a low-pass LC filter to meet output requirements (resistive load or DC output).

### 6.7 Layout Guidelines

- **PCB material:** FR-4 glass epoxy, 2 oz. copper on top and bottom, recommended for thermal performance and lower trace inductance.
- **Ground plane:** Use one large unbroken ground plane for the whole board (typically bottom layer). Keep ground-pin traces short/wide before connecting to the ground plane via multiple vias (reduces via impedance). Clear space around the device, especially on the bottom side, to improve heat spreading.
- **Decoupling capacitors:** Place 100 nF high-frequency decoupling caps on PVDD_x pins close to the pins, with a short ground return path.
- **AGND handling:** AGND is a localized ground for logic signals. Recommend a 1-Ω resistor between GND and AGND to isolate board-ground noise from AGND. Two other components reference this local ground: 0.1 µF cap from VREG to AGND, and the OC_ADJ programming resistor to AGND. Place the VREG capacitor close to the VREG/AGND pins, connected without vias.
- **Current shunt resistor placement:** If a shunt is used between GND_x↔GND or PVDD_x↔PVDD, ensure only one single path connects each GND_x/PVDD_x pin to the shunt, and keep the path short and symmetrical on each sense line to minimize measurement error from added trace resistance.
- **DRV8412 thermal via footprint (Figure 23 in source):** 4×8 array of 0.3 mm drill vias at 1 mm pitch under the thermal pad; solder-mask/pad detail 0.55×2.15 mm mask over 0.3×1.9 mm pad; additional 0.65 mm pad/mask vias (0.3 mm drill) and 0.69 mm pad/mask via (0.635 mm drill) — see original PDF Figure 23 for exact via placement.

### 6.8 Thermal Considerations

- DRV8432's thermally-enhanced package is designed to interface directly to a heat sink via thermal interface compound (e.g., Ceramique from Arctic Silver, TIMTronics 413). Good practice: connect the heat sink to system ground to reduce ground noise.
- System R_θJA = R_θJC (junction-to-case/power-pad/heat-slug) + thermal-grease thermal resistance + heat-sink thermal resistance.
- Thermal grease resistance = (grease manufacturer's area thermal resistance, °C·in²/W or °C·mm²/W) ÷ exposed heat-slug area. DRV8432 (36-pin PSOP3) exposed heat-slug size ≈ 0.124 in² (80 mm²).
- Thermal pads have higher thermal resistance than a thin grease layer and are NOT recommended; thermal tape has even higher resistance and should NOT be used at all.
- Heat-sink thermal resistance is vendor-predicted (CFD-modeled) or measured.
- Reference: TI application report *IC Package Thermal Metrics*, SPRA953.

---

## 7. Revision History Summary (condensed)

| From → To | Key changes |
|---|---|
| Rev F (Jan 2014) → Rev G (Dec 2014) | Added ESD Ratings table, Feature Description section, Device Functional Modes, Application/Implementation section, Power Supply Recommendations, Layout section, Device/Doc Support section, Mechanical/Packaging/Orderable section |
| Rev E (Oct 2013) → Rev F | Removed "requires close decoupling capacitor to ground" wording from GND_A–D pin descriptions; added "for charging the Bootstrap capacitor" to t_ON_MIN description; added note to OC Protection section |
| Rev D (Jul 2011) → Rev E | Added last sentence to Thermal Pad description; added Thermal Information table; added paragraph on DC logic level in functional modes section |
| Rev C (May 2010) → Rev D | R_DS(on) spec changed 80 mΩ → 110 mΩ; second Feature changed 50 V → 52 V (70 V abs-max deleted); added LED Lighting Drivers application; added metallization/bond-wire/pin-resistance note to R_DS(on) test conditions |
| Rev B (Jan 2010) → Rev C | Removed all DRV8422 content (part discontinued/not part of this datasheet); changed DRV8432 pinout; added Thermal Pad/Heat Slug rows to pin table; added 2nd paragraph to Bootstrap Capacitor section; removed "or GVDD undervoltage" from Device Reset section |
| Rev A (Dec 2009) → Rev B | Added T_A=125°C, 1.0 W power rating to package power derating table |

---

## 8. Package / Mechanical (DRV8432, DKD0036A — PowerPAD SSOP, 3.6 mm max height)

Key dimensions (mm, per JEDEC-style outline drawing DKD0036A, rev 4222166/A, Dec 2015):

- Body width: 5.8–5.9 mm (with 10.9–11.1 mm across the powerpad flange, note 4)
- Body length: 15.8–16.0 mm (note 3)
- Pin pitch: 34× 0.65 mm
- Lead width: 36× 0.38/0.25 mm
- Seating-plane max height: 3.6 mm (min 3.1 mm)
- Exposed thermal pad: 2× 11.05 mm reference dimension, recessed 0.15 mm from seating plane
- Gage plane lead form: 0°–8° foot angle, 1.1/0.8 mm foot length, 0.3/0.1 mm foot thickness
- Land pattern (recommended): 36× 2.0 mm × 0.45 mm pads at 0.65 mm pitch; solder mask opening per IPC-7351 (non-solder-mask-defined pads shown in source); stencil: 36× 2.0×0.45 mm openings, ~0.125 mm thick stencil, trapezoidal/rounded apertures recommended per IPC-7525.
- PowerPAD is a TI trademark.

(Full dimensioned drawing, land pattern, and stencil design are in the original PDF, pages showing "GENERIC PACKAGE VIEW," "PACKAGE OUTLINE," "EXAMPLE BOARD LAYOUT," and "EXAMPLE STENCIL DESIGN" for DKD0036A — reproduce those pages directly if exact CAD dimensions are needed.)

---

## 9. Tape & Reel / Packaging Materials

| Device | Package | Pins | SPQ | Reel Diameter (mm) | Reel Width W1 (mm) | A0 (mm) | B0 (mm) | K0 (mm) | P1 (mm) | W (mm) | Pin1 Quadrant |
|---|---|---|---|---|---|---|---|---|---|---|---|
| DRV8412DDWR | HTSSOP/DDW | 44 | 2000 | 330.0 | 24.4 | 8.6 | 15.6 | 1.8 | 12.0 | 24.0 | Q1 |
| DRV8432DKDR | HSSOP/DKD | 36 | 500 | 330.0 | 24.4 | 14.7 | 16.4 | 4.0 | 20.0 | 24.0 | Q1 |

| Device | Package | Pins | SPQ | Box Length (mm) | Box Width (mm) | Box Height (mm) |
|---|---|---|---|---|---|---|
| DRV8412DDWR | HTSSOP/DDW | 44 | 2000 | 367.0 | 367.0 | 45.0 |
| DRV8432DKDR | HSSOP/DKD | 36 | 500 | 367.0 | 367.0 | 45.0 |

---

## 10. Quick-Reference Cross Checks (for firmware/hardware integration)

- **To use CBC current limiting:** Set M1M2M3 = 000 (dual FB / 4HB) or 011 (dual FB complementary). Never hold a controlled half-bridge's PWM input at a static logic level in CBC mode — the driver won't recover from a CBC event without a PWM toggle.
- **To use OC latching shutdown (fail-safe/no CBC):** Set M1M2M3 = 001. After an OC event, toggle RESET_AB and/or RESET_CD (low→high) to clear.
- **To use parallel mode (combine 2 half-bridges per phase for higher current):** Set M1M2M3 = 010. Requires 30–100 nH inductor/ferrite bead between OUT_A/OUT_B (and OUT_C/OUT_D) before tying them together. PWM_C, PWM_D unused → tie to ground.
- **Reading FAULT/OTW together tells you the fault category** — see Table 1 in Section 4.3.
- **After ANY OCSD or OTSD event, wait ≥1 s before issuing the external reset** for best reliability.
- **OTSD requires clearing BOTH RESET_AB and RESET_CD**, unlike per-bridge OC faults which can be cleared independently.
- **Bootstrap capacitors:** 100 nF (X5R+) for 10 kHz–500 kHz switching; go larger below 10 kHz; add 5 Ω between GVDD and GVDD_X if bootstrap cap > 220 nF.
- **Short-circuit protection is not guaranteed for a bare output short** — a series inductor or ferrite bead at each output is required for the OC/CBC circuitry to work as intended under a hard short.

---

*End of converted datasheet. Source: DRV8412, DRV8432 datasheet, Texas Instruments, SLES242G, Dec 2014. © 2009–2014 Texas Instruments Incorporated. This conversion reorganizes and restates the technical content of the original document for engineering/agent reference use; consult the original PDF for exact schematics, waveform figures, and mechanical CAD drawings.*
