exists(../settings.pro) {
  include(../settings.pro)
}
else {
  error("settings.pro missing, unable to build")
}
TEMPLATE = subdirs
CONFIG *= warn_off
CONFIG *= ordered
SUBDIRS += sample_add qsac abanq_remote