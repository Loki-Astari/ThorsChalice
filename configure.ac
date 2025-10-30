AC_INIT([ThorsChalice], [0.1], [loki.Astari+ThorsChalice@gmail.com])
AC_PREREQ([2.65])

AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])

AX_THOR_FUNC_INIT_BUILD([ThorsChalice], [src/ThorsChalice/ThorsChalice.cpp])


#
# Add extra configuration here
#
AX_THOR_CHECK_USE_THORS_SERIALIZE
AX_THOR_CHECK_USE_MAGIC_ENUM
AX_THOR_CHECK_USE_THORS_NISSE

LT_INIT

AX_THOR_FEATURE_HEADER_ONLY_VARIANT([THORSCHALICE])
AX_THOR_FUNC_TERM_BUILD([THORSCHALICE], [src/ThorsChalice/ThorsChaliceConfig.h:config.h.in])

AC_OUTPUT
