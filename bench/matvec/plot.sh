#!/bin/bash
cd "$(dirname "$0")" || exit 1

# Create virtualenv if it doesn't exist
if [ ! -d ".venv" ]; then
  python3 -m venv .venv
fi

# Activate virtualenv
source .venv/bin/activate

# Install requirements quietly if not installed
pip install -q pandas matplotlib

# Run plot script silently
python3 plot.py >/dev/null 2>&1

# Confirm output
if [ -f "matvec_plot.png" ] || ls matvec_plot*.png >/dev/null 2>&1; then
  echo "✅ Plot generated successfully."
else
  echo "❌ Plot generation failed."
fi

# Deactivate env
deactivate