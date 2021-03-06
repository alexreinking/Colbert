#############################################################################
# Makefile for building: Colbert
# Generated by qmake (2.01a) (Qt 4.6.3) on: Sun Oct 17 17:36:05 2010
# Project:  Colbert.pro
# Template: subdirs
# Command: /usr/bin/qmake -unix -o Makefile Colbert.pro
#############################################################################

first: make_default
MAKEFILE      = Makefile
QMAKE         = /usr/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
SUBTARGETS    =  \
		sub-src-app \
		sub-src-plugins

src/app/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) src/app/ || $(MKDIR) src/app/ 
	cd src/app/ && $(QMAKE) /home/alex/Development/C++/Qt/Colbert/src/app/app.pro -unix -o $(MAKEFILE)
sub-src-app-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) src/app/ || $(MKDIR) src/app/ 
	cd src/app/ && $(QMAKE) /home/alex/Development/C++/Qt/Colbert/src/app/app.pro -unix -o $(MAKEFILE)
sub-src-app: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE)
sub-src-app-make_default: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) 
sub-src-app-make_first: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) first
sub-src-app-all: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) all
sub-src-app-clean: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) clean
sub-src-app-distclean: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) distclean
sub-src-app-install_subtargets: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) install
sub-src-app-uninstall_subtargets: src/app/$(MAKEFILE) FORCE
	cd src/app/ && $(MAKE) -f $(MAKEFILE) uninstall
src/plugins/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) src/plugins/ || $(MKDIR) src/plugins/ 
	cd src/plugins/ && $(QMAKE) /home/alex/Development/C++/Qt/Colbert/src/plugins/plugins.pro -unix -o $(MAKEFILE)
sub-src-plugins-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) src/plugins/ || $(MKDIR) src/plugins/ 
	cd src/plugins/ && $(QMAKE) /home/alex/Development/C++/Qt/Colbert/src/plugins/plugins.pro -unix -o $(MAKEFILE)
sub-src-plugins: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE)
sub-src-plugins-make_default: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) 
sub-src-plugins-make_first: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) first
sub-src-plugins-all: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) all
sub-src-plugins-clean: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) clean
sub-src-plugins-distclean: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) distclean
sub-src-plugins-install_subtargets: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) install
sub-src-plugins-uninstall_subtargets: src/plugins/$(MAKEFILE) FORCE
	cd src/plugins/ && $(MAKE) -f $(MAKEFILE) uninstall

Makefile: Colbert.pro  /usr/share/qt4/mkspecs/default/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -unix -o Makefile Colbert.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake: qmake_all FORCE
	@$(QMAKE) -unix -o Makefile Colbert.pro

qmake_all: sub-src-app-qmake_all sub-src-plugins-qmake_all FORCE

make_default: sub-src-app-make_default sub-src-plugins-make_default FORCE
make_first: sub-src-app-make_first sub-src-plugins-make_first FORCE
all: sub-src-app-all sub-src-plugins-all FORCE
clean: sub-src-app-clean sub-src-plugins-clean FORCE
distclean: sub-src-app-distclean sub-src-plugins-distclean FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-src-app-install_subtargets sub-src-plugins-install_subtargets FORCE
uninstall_subtargets: sub-src-app-uninstall_subtargets sub-src-plugins-uninstall_subtargets FORCE

sub-src-app-check: src/app/$(MAKEFILE)
	cd src/app/ && $(MAKE) check
sub-src-plugins-check: src/plugins/$(MAKEFILE)
	cd src/plugins/ && $(MAKE) check
check: sub-src-app-check sub-src-plugins-check

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

