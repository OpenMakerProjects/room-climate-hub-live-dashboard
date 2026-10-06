# Verified cloud validation — 2026-10-06

- Legacy metadata validator: passed.
- Native C++ control tests (g++ C++11, Wall/Wextra/Werror): passed.
- SVG XML and cross-file GPIO21/22/26/27 consistency: passed; SVG rendered/inspected.
- JSON schema/sample, MIT text, relative links and credential-pattern checks: passed.
- Image transport tests: 3 passed, including malformed base64, PNG CRC/trailer rejection and cleanup.
- GitHub Actions run 37408029403: PNG validation/decoding, same-branch push with runtime GITHUB_TOKEN, full completion gates and ESP32 board build all passed.
- Decoded branch commit: cbbb55ac65058b6c6186d8a179f9b43638a17c9e.
- PNG: 1,885,928 bytes, 1536×1024 RGB; SHA256 ea59b12b135a74bd46eee008ec02bc37c8ed0e45e8ef69eb0f6d3705f22e4a9f.
- Temporary base64 chunks removed; original PNG bytes preserved, including public image provenance. No credential patterns were found in PNG bytes; no private credentials were supplied or committed. The documented AP password is explicitly public demo configuration.

No physical hardware, electrical or radio testing was performed. The earlier direct binary upload rejection was resolved through the user-authorized text-to-Actions recovery. No force push, stored PAT or skip-ci marker was used.
