---
description: Walk me through the branch → commit → PR → review → merge loop
argument-hint: [what you're shipping, or blank to work it out from the diff]
---

Shipping: ${ARGUMENTS:-whatever is in my working tree — check git status and git diff}

Walk me through the professional loop. **Talk me through each step and let me run the commands
myself** — I'm learning Git, so I need the reps. Explain what each command does the first few
times, then stop narrating once it's clearly stuck.

1. **Check state.** Where am I? Am I on `main` (I shouldn't be)? What's uncommitted?
   If I've already committed to `main` by mistake, teach me how to move those commits onto a
   branch rather than doing it for me.

2. **Branch.** Suggest a name following a sensible convention (`fix/`, `feat/`, `docs/` +
   short description). Tell me the convention, not just the name.

3. **Commits.** Is this one logical change or several? If several, teach me `git add -p` to
   split them. Review my commit messages: imperative mood, why over what in the body, no
   "fixed stuff". Push back on lazy ones.

4. **Self-review before the PR.** Make me read my own `git diff` first and tell you what I
   notice. Then run `/critique` on it. Fixing my own findings before a reviewer sees them is
   the habit that makes people trust your PRs.

5. **PR.** Help me write a description that explains *why*, not just *what* — context, the
   approach, anything I'm unsure about and want a second opinion on. Then `gh pr create`.
   A good PR description is a professional skill; treat it as one.

6. **Review.** Review the PR properly, as if you didn't write any of it. Be honest — a rubber
   stamp teaches me nothing.

7. **Merge and clean up.** Merge, delete the branch locally and remotely, pull `main`.
   Close the issue if it isn't auto-closed.

If anything here is already set up correctly, just say so and move on — don't make me do
ceremony for its own sake.
