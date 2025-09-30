arch-meson \
    --wrap-mode=default \
    --buildtype=release \
    -Dxwayland=auto \
    -Duse_system_wlroots=enabled \
    -Duse_system_wfconfig=disabled \
    -Db_lto=true \
    -Db_pie=true \
    -Dtests=disabled \
    build
ninja -C build
