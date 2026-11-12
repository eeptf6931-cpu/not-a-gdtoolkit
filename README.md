# GD Toolkit

A Geode mod project for Geometry Dash 2.2081. The initial commit establishes the cross-platform build skeleton and a small in-game frame-update diagnostic.

## Targets
- Windows x64 (Geode target: `Win64`)
- Android ARM64 (Geode target: `Android64`)

## Current status
**Early development — not a finished FB Counter / Frame Window replacement.**

Implemented in this bootstrap:
- Geode mod metadata and settings
- PlayLayer update counter
- Optional periodic diagnostic messages in the Geode log
- GitHub Actions build jobs for Windows x64 and Android ARM64

Not implemented yet:
- Per-click frame-window buckets (1–10 frames)
- Click/event detection and visual HUD
- Attempt history and statistics
- Macro import/export and click editor
- Full settings GUI and visual customization

The update counter counts PlayLayer update calls; it is not a verified physical-frame or input-to-input frame-window measurement. These require dedicated input/event instrumentation and in-game validation.

## Build
GitHub Actions builds both targets and uploads resulting `.geode` packages as workflow artifacts.

## Development
This project uses [geode-sdk/build-geode-mod](https://github.com/geode-sdk/build-geode-mod).

Repository: https://github.com/eeptf6931-cpu/not-a-gdtoolkit
