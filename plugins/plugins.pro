TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency \
    hour_signal \
    alarm \
    tray_color \
    any_zoom \
    schedule \
    quick_note \
    date \
    move_clock \
    tower_clock_signal

windows {
SUBDIRS += \
    power_off \
    win_on_top
}
