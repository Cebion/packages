#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2021-present Cebion

INSTALLPATH="/roms/ports"
PKG_NAME="f2bgl"
PKG_FILE="f2bgl-gamedata.zip"
PKG_VERSION="1.6"
PKG_SHASUM="257dd158d0e315650fb0fc4021959a3ac551c38bfed845d976e51bc93877eeb5"
SOURCEPATH=$(pwd)
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
cat <<EOF >${INSTALLPATH}/"F2bgl.sh"
#!/bin/bash
source /etc/profile
/usr/bin/show_splash.sh "Fade to Black"
clear >/dev/console

jslisten set "f2bgl"

export LD_LIBRARY_PATH=${INSTALLPATH}/${PKG_NAME}/lib:/usr/lib
cd ${INSTALLPATH}/${PKG_NAME}
LIBGL_FB=2 LIBGL_ES=2 LIBGL_GL=21 LIBGL_NOTEST=1  ./f2bgl

jslisten stop

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

for image in system-f2bgl-thumb.png system-f2bgl.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

for video in system-f2bgl.mp4
do
  cp "${SOURCEPATH}/${PKG_NAME}/${video}" "${INSTALLPATH}/videos"
done

### Add Game to the game list XML
if [ ! "$(grep 'Fade to Black' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './F2bgl.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'Fade to Black'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'F2bgl is a rewrite of the engine used in the game Fade To Black.'\
		-s '//gameList/game[last()]' -t elem -n 'genre'       -v 'Action'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-f2bgl.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-f2bgl-thumb.png'\
                -s '//gameList/game[last()]' -t elem -n 'video'       -v './videos/system-f2bgl.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '4.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '1995'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Delphine Software'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'Electronic Arts'\
		${INSTALLPATH}/gamelist.xml
fi
