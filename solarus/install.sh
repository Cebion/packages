#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/solarus"
PKG_NAME="solarus"
PKG_FILE="solarus.zip"
PKG_VERSION=1.0.0"
PKG_SHASUM="e8eb7e997fb8478b04a1c28a32a1ad84708a586a524bd256d7c4df41c8977029"
SOURCEPATH=$(pwd)

### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}/${PKG_NAME}

curl -Lo ${PKG_FILE} https://github.com/cebion/packages/releases/download/${PKG_VERSION}/${PKG_FILE}
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
if [ ! "$(grep -q 'Solarus' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Solarus.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'Solarus'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'Solarus is a free and open-source 2D game engine written in c++.'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-solarus.png'\
		-s '//gameList/game[last()]' -t elem -n 'video'       -v './images/system-solarus.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-solarus-thumb.png'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '1.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '2020'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Solarus'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'Solarus'\
		${INSTALLPATHINSTALLPATH}/gamelist.xml
fi
