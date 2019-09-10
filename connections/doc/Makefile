DOXYCOMM = Doxyfile

.PHONY: doxygen
doxygen: $(DOXYCOMM)
	doxygen $(DOXYCOMM)
	echo 'include : ["_*.html"]' >> html/_config.yml

.PHONY: clean
clean:
	rm -rf html
