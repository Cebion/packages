#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/gamedata"
PKG_NAME="solarus"
PKG_FILE="solarus-gamedata.zip"
PKG_VERSION="1.0"
PKG_SHASUM="8bce037e4e56ab2c6e3aacc171355122f85e3e17e220170e102260b2a0f25f91"
SOURCEPATH=$(pwd)
CFG="/storage/.emulationstation/es_systems.cfg"
START_SCRIPT="$BINARY.sh"
BINARY="./bin/solarus-run"

### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}/${PKG_NAME}

curl -Lo ${PKG_FILE} https://github.com/Cebion/packages/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}

### Create the start script
cat <<EOF >${INSTALLPATH}/${PKG_NAME}/"solarus.sh"
#!/bin/bash
source /etc/profile
export LD_LIBRARY_PATH=${INSTALLPATH}/${PKG_NAME}/data:/usr/lib
cd ${INSTALLPATH}/${PKG_NAME}
  ./bin/solarus-run -joypad-deadzone=32767 -quit-combo=6+7 \$1

EOF

### Add Solarus images
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

for image in system-solarus.png  system-solarus-thumb.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

### Add Solarus to the game list
if grep -q '<name>solarus</name>' "$CFG"
then
	xmlstarlet ed -L -P -d "/systemList/system[name='solarus']" $CFG
fi

	echo 'Adding Solarus to systems list'
	xmlstarlet ed --omit-decl --inplace \
		-s '//systemList' -t elem -n 'system' \
		-s '//systemList/system[last()]' -t elem -n 'name' -v 'solarus'\
		-s '//systemList/system[last()]' -t elem -n 'fullname' -v 'Solarus Game Engine'\
		-s '//systemList/system[last()]' -t elem -n 'path' -v '/storage/roms/solarus'\
		-s '//systemList/system[last()]' -t elem -n 'manufacturer' -v 'Solarus'\
		-s '//systemList/system[last()]' -t elem -n 'release' -v '2020'\
		-s '//systemList/system[last()]' -t elem -n 'hardware' -v 'Game Engine'\
		-s '//systemList/system[last()]' -t elem -n 'extension' -v '.solarus'\
		-s '//systemList/system[last()]' -t elem -n 'command' -v '/storage/roms/gamedata/solarus/solarus.sh %ROM%'\
		-s '//systemList/system[last()]' -t elem -n 'platform' -v 'solarus'\
		-s '//systemList/system[last()]' -t elem -n 'theme' -v 'solarus'\
		$CFG
