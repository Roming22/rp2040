UF2=$(find /tmp/rarnaud/arduino/sketches -name \*.uf2 | xargs ls -t | head -1)

while [ ! -d "$HOME/mnt/RPI-RP2/" ]; do
	sleep 1;
done
echo "Copying '$UF2' to board"
sleep 1;
cp "$UF2" "$HOME/mnt/RPI-RP2/"
