# $OpenBSD: Makefile,v 1.2 2007/03/30 03:55:28 todd Exp $

# Various list maintenance functions

#   sort: verify each md/mi file is sorted and contains uniq lines
# mdtomi: move common md file lines to mi, if present
#  dupes: detect (nothing more) duplicate entries in each arch's complete file
#         list


all: sort


sort:
	@for f in */m*; do echo "===> sorting $$f"; \
		sed 's/[ ]*$$//' $$f | sort | uniq > $$f.tmp; \
		diff -u $$f $$f.tmp || true; \
		cat $$f.tmp > $$f && rm $$f.tmp; \
	done

basefiles!= cd xbase;echo md.*
archs  = ${basefiles:S/md\.//}

mdtomi:
	@echo "Checking for common md file lines for migration to mi files:"
	@dir=$$(mktemp -d /tmp/mdtomi.XXXXXXXXXX); \
	i1=first; \
	for a in ${archs}; do \
		sort */md.$$a >> $$dir/$$a; \
		[ "X$$i1" = "Xfirst" ] && { \
			i1=$$dir/$$a; \
		} || { \
			cat $$i1 $$dir/$$a | sort | uniq -d >> $$dir/mi.tmp; \
			echo "===> ($$(echo $$(cat $$dir/mi.tmp|wc -c)))\c"; \
			echo " $$i1 vs $$dir/$$a"; \
			mv $$dir/mi.tmp $$dir/mi; \
			i1=$$dir/mi; \
		}; \
	done; \
	echo "If any common md files were found, migrating to mi files:"; \
	cat $$dir/mi | while read line; do \
		echo "==> $$line"; \
		for line2 in $$(grep $$line */m*); do \
			f=$${line2%:*}; \
			grep -v $$line $$f > $$f.tmp && mv $$f.tmp $$f; \
			d=$${f%/*}; \
			echo "===> -$$line2"; \
		done; \
		echo "===> +$$d/mi"; \
		echo $$line >> $$d/mi; \
	done; rm -rf $$dir

dupes:
	@for arch in ${archs}; do \
		echo "===> $$arch"; \
		cat */md.$$arch */mi | sort | uniq -d; \
	done

