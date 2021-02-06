#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2020-present Cebion

INSTALLPATH="/storage/roms/ports"
PKG_NAME="cdogs-sdl"
PKG_FILE="cdogs-sdl-gamedata.zip"
PKG_VERSION="1.0"
PKG_SHASUM="0bd84843c2861c119e44eecfc567799f0174a7179e14effecc543ccdaede6a83"
SOURCEPATH=$(pwd)
### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}/${PKG_NAME}

curl -Lo ${PKG_FILE} https://github.com/Cebion/cdogs-sdl/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}

### Create the start script
cat <<EOF >${INSTALLPATH}/"Cdogs-sdl.sh"
#!/bin/bash
source /etc/profile
/usr/bin/show_splash.sh "C-Dogs SDL"
clear >/dev/console
cd /storage/roms/ports/cdogs-sdl/
 ./cdogs-sdl

EOF

### Add images if not exist folder
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

### Add videos if not exist folder
if [ ! -d "${INSTALLPATH}/videos" ]
then
  mkdir -p "${INSTALLPATH}/videos"
fi

for image in system-cdogs-thumb.png system-cdogs.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

for video in system-cdogs.mp4
do
  cp "${SOURCEPATH}/${PKG_NAME}/${video}" "${INSTALLPATH}/videos"
done

### Add C-Dogs to the game list
if [ ! "$(grep -q 'C-Dogs' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Cdogs-sdl.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'C-Dogs'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'C-Dogs SDL is a classic overhead run-and-gun game.'\
		-s '//gameList/game[last()]' -t elem -n 'genre'       -v 'Action'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-cdogs.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-cdogs-thumb.png'\
                -s '//gameList/game[last()]' -t elem -n 'video'       -v './videos/system-cdogs.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '4.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '1997'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'NN'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'NN'\
		${INSTALLPATH}/gamelist.xml
fi
