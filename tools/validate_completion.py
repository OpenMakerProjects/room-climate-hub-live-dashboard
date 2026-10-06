"""Validate required assets and cross-file pin consistency. Run from repo root."""
import argparse, json, pathlib, re, subprocess, tempfile
import xml.etree.ElementTree as ET
root = pathlib.Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser()
p.add_argument('--allow-pending-image', action='store_true')
args = p.parse_args()
svg = root / 'docs/circuit-diagram.svg'
ET.parse(svg)
firmware = (root/'firmware/room-climate-hub-live-dashboard/room-climate-hub-live-dashboard.ino').read_text()
readme = (root/'README.md').read_text()
for name,pin in [('PIR_PIN',27),('RELAY_PIN',26),('SDA_PIN',21),('SCL_PIN',22)]:
    assert re.search(rf'{name}\s*=\s*{pin}\b',firmware), name
    assert f'GPIO{pin}' in svg.read_text() and f'GPIO{pin}' in readme, name
sample=json.loads((root/'sample-data/dashboard-example.json').read_text())
assert sample['project_id']==1 and sample['sensor_ok'] is True
assert 'MIT License' in (root/'LICENSE').read_text()
with tempfile.TemporaryDirectory() as t:
    binary=pathlib.Path(t)/'control_test'
    subprocess.run(['g++','-std=c++11','-Wall','-Wextra','-Werror',str(root/'tests/control_test.cpp'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)
for path in ['README.md','docs/wiring.md','docs/architecture.md','docs/test-plan.md','docs/circuit-diagram.svg']:
    text=(root/path).read_text()
    assert not re.search(r'(ghp_[A-Za-z0-9]{20,}|-----BEGIN .*PRIVATE KEY)',text), path
for link in re.findall(r'\]\(([^)]+)\)',readme):
    if not link.startswith(('https://','http://','#')):
        if link=='docs/images/project-overview.png' and args.allow_pending_image: continue
        assert (root/link).exists(), f'Broken relative link: {link}'
image=root/'docs/images/project-overview.png'
if args.allow_pending_image:
    print('PARTIAL PASS: C++ behavior, pin/SVG consistency, JSON, MIT, links, basic secret patterns; image gate explicitly deferred')
else:
    assert image.exists() and image.read_bytes().startswith(b'\x89PNG\r\n\x1a\n'), 'Required PNG missing'
    print('PASS: completion assets and host validation')

# Strict final PNG validation and lossless manifest match; transports must be gone.
if not args.allow_pending_image:
    import hashlib
    from decode_project_image import validate_png
    manifest=json.loads((root/'docs/images/project-overview.manifest.json').read_text())
    data=image.read_bytes()
    assert list(validate_png(data)) == manifest['dimensions']
    assert len(data)==manifest['bytes'] and hashlib.sha256(data).hexdigest()==manifest['sha256']
    assert not list((root/'docs/images').glob('project-overview.png.b64.*')), 'Temporary transport remains'
