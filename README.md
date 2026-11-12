# GD Toolkit

A Geometry Dash 2.2081 Geode mod for Windows x64 and Android ARM64.

## Implemented in the current development build

- Live in-game HUD with a click counter and session attempt counter.
- Frame-window histogram measured from actual jump-button input callbacks:
  - 1 frame
  - 2 frames
  - 3–4 frames
  - 5–6 frames
  - 7–8 frames
  - 9–10 frames
  - 11+ frames
- Best progress tracking for the current level session.
- A **Toolkit** button in the pause menu that opens a session report.
- An **Export** button that writes session statistics and a bounded, attempt-indexed input-event timeline to JSON/CSV in the mod save folder.
- Geode settings for enabling the tracker, showing the HUD, and changing HUD scale.

## How measurements work

A frame window is the number of game update frames between two consecutive jump presses. It is not inferred from displayed FPS. The HUD updates several times per second to keep overhead low.

The current implementation tracks player 1's primary jump input. Dual-player jump-window statistics, ring/orb classification, per-level persistent history, macro playback/import, click markers, and the full multi-tab editor UI are not implemented yet. Input events from both players are included in the exported timeline; the click/frame-window summary currently focuses on player 1's primary jump input.

## Build

The GitHub Actions workflow builds:
- Windows x64 (`Win64`)
- Android ARM64 (`Android64`)

Install the generated `.geode` package that matches your platform from the workflow artifacts once both jobs complete successfully.

## Status

This is an active development build, not a finished release. A successful package build and in-game testing are required before calling a version stable.
