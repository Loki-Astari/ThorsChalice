THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = ThorsSlackBot.slib SlackCLI.prog
TEST_ONLY					= YES
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor
LDLIBS_EXTERN_BUILD			= Nisse ThorSerialize

include $(THORSANVIL_ROOT)/build/tools/Makefile

done:
	echo "Stopping Chalice"
	curl localhost:8079 2> /dev/null || echo "Done"
