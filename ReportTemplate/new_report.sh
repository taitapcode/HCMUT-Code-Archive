#!/usr/bin/env bash
# Scaffold a new HCMUT report from this template.
#
#   ./new_report.sh Lab1              -> Lab1/report   (Lab1 already exists)
#   ./new_report.sh path/to/report    -> used verbatim
#   ./new_report.sh -f Lab1           -> overwrite an existing report/
#   TEMPLATE=/other ./new_report.sh x -> use a different template
#
# Copies the template, then drops Sections/01..06 (demo), main.pdf, README.md,
# build artefacts (.aux .toc .log .out .synctex.gz .tectonic) and .git.
set -euo pipefail

TEMPLATE="${TEMPLATE:-$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)}"
force=0
arg=""

for a in "$@"; do
  case "$a" in
    -f|--force) force=1 ;;
    -h|--help) sed -n '2,10p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit 0 ;;
    *) arg="$a" ;;
  esac
done

[[ -n "$arg" ]] || { echo "usage: $(basename "$0") [-f] <dir-or-parent-dir>" >&2; exit 1; }
[[ -f "$TEMPLATE/main.tex" ]] || { echo "not a template: $TEMPLATE" >&2; exit 1; }

# Existing parent dir -> create <dir>/report; new path -> use as given.
if [[ -d "$arg" && "$(basename "${arg%/}")" != "report" ]]; then
  dest="${arg%/}/report"
else
  dest="$arg"
fi

if [[ -e "$dest" ]]; then
  (( force )) || { echo "already exists: $dest (use -f to overwrite)" >&2; exit 1; }
  rm -rf -- "$dest"
fi

mkdir -p "$dest"
cp -r "$TEMPLATE"/. "$dest"/

# Drop demo content + build junk
rm -rf -- "$dest/Sections/0"[1-9]_*.tex "$dest/.git" "$dest/.tectonic" \
          "$dest/main.pdf" "$dest/README.md"
rm -f -- "$dest/$(basename "${BASH_SOURCE[0]}")"
find "$dest" -maxdepth 1 -type f \
  \( -name '*.aux' -o -name '*.toc' -o -name '*.log' -o -name '*.out' \
     -o -name '*.synctex.gz' -o -name '*.fls' -o -name '*.xdv' \) -delete

# main.tex: collapse the six demo \input lines into one commented placeholder.
awk '
  /^\\input\{Sections\/0[1-9]_/ {
    if (!done) { print "% \\input{Sections/01_content}"; done = 1 }
    next
  }
  { print }
' "$dest/main.tex" > "$dest/main.tex.tmp" && mv "$dest/main.tex.tmp" "$dest/main.tex"

cat <<EOF
Scaffolded report skeleton: $dest

Edit first:
  $dest/main.tex                 \\headercoursename{...}, \\footerreporttitle{...}
  $dest/Sections/00_title.tex    course, topic, instructor, class, students

Add content: create Sections/01_content.tex, then uncomment the matching
\\input line in main.tex. Syntax samples (tables, figures, algorithms, code,
theorems) live in $TEMPLATE/Sections/ - copy the parts you need.
EOF
