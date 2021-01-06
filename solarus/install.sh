#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/gamedata"
PKG_NAME="solarus"
PKG_FILE="solarus-gamedata.zip"
PKG_VERSION=1.0"
PKG_SHASUM="e8eb7e997fb8478b04a1c28a32a1ad84708a586a524bd256d7c4df41c8977029"
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

unzip -o ${PKG_NAME}
mv solarus/* .
rmdir solarus

### Create the start script
cat <<EOF >${INSTALLPATH}/"solarus.sh"
export LD_LIBRARY_PATH=${INSTALLPATH}/${PKG_NAME}:/usr/lib
cd ${INSTALLPATH}/${PKG_NAME}
  ./bin/solarus-run $1 
fi
ret_error=\$?
[[ "\$ret_error" != 0 ]] && (echo "Error executing Solarus.  Please check that you copied your Quest File to ${INSTALLPATH}/${PKG_NAME}" >/tmp/logs/emuelec.log)
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
		-s '//systemList/system[last()]' -t elem -n 'command' -v "/emuelec/scripts/$START_SCRIPT %ROM%"\
		-s '//systemList/system[last()]' -t elem -n 'platform' -v 'solarus'\
		-s '//systemList/system[last()]' -t elem -n 'theme' -v 'solarus'\
		$CFG
