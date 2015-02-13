SYSTEMC_BUILD = /dev/shm/systemc
SYSTEMC_VER   = 2.3.1
SYSTEMC_TAR   = systemc-$(SYSTEMC_VER).tgz

update_systemc: $(SYSTEMC_TAR)
	rm -rf $(SYSTEMC_BUILD); mkdir -p $(SYSTEMC_BUILD)
	cp -rp patches* $(SYSTEMC_BUILD)
	cat $(SYSTEMC_TAR) | (cd $(SYSTEMC_BUILD) > /dev/null; tar -zxvf -)
	./build_vars.sh && \
	    cd $(SYSTEMC_BUILD) > /dev/null && \
	    (cd systemc-$(SYSTEMC_VER) && \
		for p in `cat ../patches-$(SYSTEMC_VER)/order.txt`; do \
		    patch -p0 -i ../patches-$(SYSTEMC_VER)/$$p; \
		done && \
		./configure && \
		make install -j OPT_CXXFLAGS="-g -O2") && \
	    mkdir local_systemc && \
	    cp -rp systemc-$(SYSTEMC_VER)/include local_systemc && \
	    cp -rp systemc-$(SYSTEMC_VER)/lib-* local_systemc
	rm -rf local_systemc && mv $(SYSTEMC_BUILD)/local_systemc .

$(SYSTEMC_TAR):
	@echo "Please download $@ into the import directory"
	@exit

clean:
	rm -rf $(SYSTEMC_BUILD) local_systemc