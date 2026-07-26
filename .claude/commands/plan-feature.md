---
description: Break a feature into a professional plan and GitHub issues
argument-hint: [the feature]
---

Feature: $ARGUMENTS

Help me plan this the way a professional team would. **Planning only — write no implementation
code.**

Work through it in this order:

1. **Clarify.** Ask me anything genuinely ambiguous about scope or intent before planning.
   Don't guess at requirements — that's a habit worth breaking early.

2. **Design options.** Give me 2–3 genuinely different approaches, each with real tradeoffs
   (complexity, memory cost, how well it survives the move to embedded hardware, how hard it is
   to change later). Recommend one and say why — but the decision is mine.

3. **Check it against the open decisions** in CLAUDE.md. Does this feature force one of them?
   If so, flag it — that decision needs making first, and probably deserves an ADR.

4. **Decompose into issues** once I've picked an approach. Each issue should be:
   - independently completable and reviewable
   - small enough for one sitting
   - written with a clear "done when..." acceptance criterion
   - titled as an imperative ("Add framebuffer bounds checking to printChar")

   Note dependencies between them and suggest an order.

5. **Identify what I should learn** from each issue — the C++ concept or practice it exercises.
   If an issue teaches nothing, it's a candidate to delegate to you.

Then offer to create the issues with `gh issue create`. Don't create them without asking.
