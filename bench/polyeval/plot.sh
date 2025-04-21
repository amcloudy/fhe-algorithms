#!/bin/bash

cd "$(dirname "$0")" || exit 1

CSV_FILE="polyeval_ring_bench.csv"

# Check CSV existence
if [ ! -f "$CSV_FILE" ]; then
  echo "❌ CSV file '$CSV_FILE' not found. Please run the benchmark first."
  exit 1
fi

# Create virtualenv if not exists
if [ ! -d ".venv" ]; then
  echo "🔧 Creating virtualenv..."
  python3 -m venv .venv
fi

# Activate virtualenv
source .venv/bin/activate

# Install required packages silently
pip install -q pandas matplotlib

# Run plot script and capture output
echo "📈 Generating plot..."
python3 plot.py > plot.log 2>&1

# Check success
if [ -f "polyeval_plot.png" ]; then
  echo "✅ Plot generated successfully: polyeval_plot.png"
else
  echo "❌ Plot generation failed. Check plot.log for errors."
fi

# Deactivate
deactivate
