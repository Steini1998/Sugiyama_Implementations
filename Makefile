linking_libs := user32.lib gdi32.lib msimg32.lib comdlg32.lib shell32.lib advapi32.lib wsock32.lib

all: bin/demo.exe
	

bin/demo.exe: intermediates/demo.obj intermediates/input.obj intermediates/cycle_breaking_berger.obj \
				intermediates/leveling.obj intermediates/remake_graph.obj intermediates/new_nodes.obj \
				intermediates/panel_ui.obj				
	cl -nologo -MT $^ -Fe$@ .\\lib\\leda.lib .\\lib\sugiyama.lib $(linking_libs)
	
bin/demo_only_cycle_break.exe: intermediates/demo_only_cycle_break.obj intermediates/cycle_breaking_berger.obj \
								intermediates/cmd_ui.obj
	cl -nologo -MT $^ -Fe$@ .\\lib\\leda.lib .\\lib\sugiyama.lib $(linking_libs)

intermediates/%.obj: src/steps/%.cpp incl/SUGIPROJ/steps/%.h
	cl -nologo -W3 -c -Zm300 -TP -EHsc -Ox -MT -I .\\incl $< -Fo$@

intermediates/%.obj: src/user_interfaces/%.cpp incl/SUGIPROJ/user_interfaces/%.h
	cl -nologo -W3 -c -Zm300 -TP -EHsc -Ox -MT -I .\\incl $< -Fo$@

intermediates/%.obj: src/%.cpp
	cl -nologo -W3 -c -Zm300 -TP -EHsc -Ox -MT -I .\\incl $< -Fo$@

.PRECIOUS: intermediates/%.obj
	
.PHONY: clean
clean:
	echo "Nothing"