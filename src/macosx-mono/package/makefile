

package:
	[ -d Distribution.save ] || make dist
	open msPackage.pmsp

dist:
	./scripts/makeContent
	sudo ./scripts/finalize
		
clean:
	sudo ./scripts/revert
	./scripts/clearContent
	