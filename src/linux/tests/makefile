
all :
	make -C ./Events
	make -C ./Filters
	make -C ./Functions
	make -C ./Drivers
	
clean :
	make -i -C ./Events clean
	make -i -C ./Filters clean
	make -i -C ./Functions clean
	make -i -C ./Drivers clean

run :
	[ -d results ] || mkdir results
	Functions/TFonctions > results/TFonctions.txt
	Events/TEvents > results/TEvents.txt
	Filters/TFilters > results/TFilters.txt
	
