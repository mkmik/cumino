all:
	rm -f _obj/_*
	$(MAKE) -f Makefile.common XENVERSION=3.0
	rm -f _obj/_*
	$(MAKE) -f Makefile.common XENVERSION=3.2
	rm -f _obj/_*
	$(MAKE) -f Makefile.common XENVERSION=3.4
	rm -f _obj/_*
	$(MAKE) -f Makefile.common XENVERSION=4.0


clean:
	$(MAKE) -f Makefile.common XENVERSION=3.0 clean
	$(MAKE) -f Makefile.common XENVERSION=3.4 clean
	$(MAKE) -f Makefile.common XENVERSION=4.0 clean
