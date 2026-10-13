# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/13 12:50:00 by msoriano          #+#    #+#              #
#    Updated: 2026/10/13 13:58:51 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Module 02 has no C++ to compile: ex00 delivers UML diagrams drawn in
# PlantUML. The .puml files are the source of truth; the .png files are
# committed, so the exercise is complete even where PlantUML is missing.
PUML := subject.puml sequence_accel.puml sequence_brake.puml sequence_steer.puml
PNGS := $(PUML:.puml=.png)

# `all` regenerates every PNG from its .puml twin
#   1. @if command -v plantuml >/dev/null 2>&1; then
#      Checks uml binary exists on PATH + `@` hides the line itself from log
#   2. plantuml -tpng $(PUML);
#      Renders the four .puml sources to .png files (-tpng) 
#   3. elif [ -n "$(PLANTUML_JAR)" ] && [ -f "$(PLANTUML_JAR)" ]; then
#      True when the caller exported & file really exists
#   4. java -jar "$(PLANTUML_JAR)" -tpng $(PUML);
#      Same render as step 2, but through an explicit jar instead of the binary
#   5. for f in $(PNGS); do
#      Loops over the four PNG names
#   6. [ -f $$f ] || { echo "missing $$f and no PlantUML available"; exit 1; };
#		- Keeps (-f = file exists) or aborts build (exit 1) with the name of the missing file.
#		- $$f is basically a single $f
#      to the shell instead of expanding it as a make variable.
#   7. fi
#      Closes if/elif/else
all:
	@if command -v plantuml >/dev/null 2>&1; then \
		plantuml -tpng $(PUML); \
	elif [ -n "$(PLANTUML_JAR)" ] && [ -f "$(PLANTUML_JAR)" ]; then \
		java -jar "$(PLANTUML_JAR)" -tpng $(PUML); \
	else \
		for f in $(PNGS); do \
			[ -f $$f ] || { echo "missing $$f and no PlantUML available"; exit 1; }; \
		done; \
		echo "ex00: no PlantUML found, committed PNGs are up to date"; \
	fi

# The PNGs are deliverables, not build artifacts: clean never removes them.
clean:
	@echo "ex00: nothing to clean (PNGs are deliverables)"

fclean: clean

re: fclean all

.PHONY: all clean fclean re
