---
description: Review code I wrote — critique it, do not fix it
argument-hint: [file, or blank for the working diff]
---

Review my code: ${ARGUMENTS:-the current working diff (git diff, and untracked files)}

**Critique only. Do not edit any files.** I want to make the fixes myself — that's the point.

For each issue found, give me:

- **Where** — `file.cpp:line`
- **Category** — correctness / memory safety / UB / design / readability / naming / performance
- **Why it matters** — the concrete failure. What input or state makes this go wrong, and what
  actually happens? "This could be a problem" is useless; "if `y` is 235, this writes past the
  end of the buffer" is useful.
- **A nudge, not a patch** — enough for me to work out the fix myself.

Order by severity: memory safety and UB first, then correctness, then design, then style.

Also tell me:

- **What I got right.** Genuinely — if I used a good idiom or handled an edge case well, say so.
  I can't build judgment if I only ever hear what's broken.
- **The one thing to fix first**, if I only do one.

If you find nothing serious, say that plainly rather than inventing minor nitpicks to fill space.
