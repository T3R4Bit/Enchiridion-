# Hardware

KiCad 8 schematic and PCB for Enchiridion.

## Design constraints (from the blueprint)

- **2-layer PCB.**
- **All SMD components on the top layer** to allow single-sided reflow and
  avoid double-sided SMT cost at the fab (JLCPCB / PCBWay).
- The **SMD 18650 holder is hand-soldered to the bottom** after board delivery.
- Export `.STEP` from KiCad into Fusion 360 so the enclosure fits precisely.

## Contents (planned)

- `enchiridion.kicad_pro` — project file
- `enchiridion.kicad_sch` — schematic
- `enchiridion.kicad_pcb` — board layout
- `gerbers/` — generated fabrication output (git-ignored)

Pin assignments: see [`../docs/project_blueprint.md`](../docs/project_blueprint.md) §4
and the firmware pin map at `../firmware/main/board_pins.h`.
