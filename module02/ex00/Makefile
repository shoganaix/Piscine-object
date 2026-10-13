# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/13 12:50:00 by msoriano          #+#    #+#              #
#    Updated: 2026/10/13 12:50:00 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Module 02 has no C++ to compile: ex00 delivers UML diagrams drawn in
# PlantUML. The .puml files are the source of truth; the .png files are
# committed, so the exercise is complete even where PlantUML is missing.
PUML := subject.puml sequence_accel.puml sequence_brake.puml sequence_steer.puml
PNGS := $(PUML:.puml=.png)

# `all` regenerates every PNG from its .puml twin. Instruction by instruction,
# top to bottom as they run when PlantUML IS installed via the binary:
#   1. `@if command -v plantuml >/dev/null 2>&1; then`
#      Checks a `plantuml` binary exists on PATH. Output is discarded, only the
#      exit code matters. The leading `@` hides the line itself from the log.
#   2. `plantuml -tpng $(PUML);`
#      Renders the four .puml sources to side-by-side .png files (`-tpng`
#      selects the PNG backend; `a.puml` always produces `a.png`).
# ...or via the fallback jar:
#   3. `elif [ -n "$(PLANTUML_JAR)" ] && [ -f "$(PLANTUML_JAR)" ]; then`
#      True when the caller exported e.g. `PLANTUML_JAR=/tmp/plantuml.jar` and
#      that file really exists (`-n` = non-empty string, `-f` = regular file).
#   4. `java -jar "$(PLANTUML_JAR)" -tpng $(PUML);`
#      Same render as step 2, but through an explicit jar instead of the binary.
# ...or, with no renderer at all, just verify the committed deliverables:
#   5. `for f in $(PNGS); do`
#      Loops over the four expected PNG names (`subject.png`, …).
#   6. `[ -f $$f ] || { echo "missing $$f and no PlantUML available"; exit 1; };`
#      Keeps (`-f` = file exists) or aborts the build (`exit 1`) with the name
#      of the missing file. `$$f` is `$$`-escaped so make passes a single `$f`
#      to the shell instead of expanding it as a make variable.
#   7. `echo "ex00: no PlantUML found, committed PNGs are up to date";`
#      Success path without a renderer: the committed PNGs are the deliverable,
#      nothing to rebuild.
#   8. `fi`
#      Closes the `if/elif/else` opened in step 1.
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
