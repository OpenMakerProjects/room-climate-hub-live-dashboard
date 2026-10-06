# Cloud validation, 2026-10-06

Executed in the cloud on 2026-10-06:
- `python tools/validate.py`: PASS.
- `python tools/validate_completion.py --allow-pending-image`: PASS (partial only).
- Native C++ control compiled with g++ C++11, Wall/Wextra/Werror and all assertions passed.
- XML parsing, firmware/README/SVG pin consistency, JSON sample, MIT text, local
  links excluding the explicitly pending image, and basic secret patterns passed.
- Full completion gate fails because the required PNG is not committed.
- Board build and physical hardware remain unverified; no hardware testing claimed.

Physical hardware has not been tested.
The firmware is intended for ESP32 DevKit using PlatformIO's esp32dev target.
A host C++ build tests actuator fault handling; it does not validate radio, I2C timing,
the full board toolchain or electrical behavior.

Required project image generation succeeded, but GitHub create_blob upload was rejected
with `user rejected MCP tool call`. The PNG has not been committed. This PR must remain
unmerged until the image is uploaded and the complete required tree passes validation.

## GitHub cloud board build

The `board` job of run 37406890175 passed PlatformIO's esp32dev build for firmware
commit 499f7450d9495d26501a2cc76d8da915a46809c4. The `host` completion job failed
at the missing PNG gate. The firmware compiled; no physical hardware was tested.
