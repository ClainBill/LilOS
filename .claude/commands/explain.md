---
description: Teach me a C++ concept, grounded in my own code
argument-hint: [concept, e.g. "RAII" or "why is this a reference"]
---

Teach me: $ARGUMENTS

Rules for this explanation:

- **Ground it in LilOS.** Find a real example in this repo where the concept applies, is
  violated, or would improve things. A concrete line of my own code beats an abstract `Foo`
  class every time. If the repo has no relevant example, say so and use the smallest possible
  synthetic one.
- **Lead with the problem it solves**, not the mechanism. I should understand *why this exists*
  before I see the syntax.
- **Show the failure mode.** What goes wrong without it? If it's undefined behaviour, say
  explicitly what UB means here — that it may appear to work fine and then break later.
- **Flag it if I'm already getting it wrong** somewhere in the repo.
- **Keep it tight.** One concept, well explained, beats a survey.
- **End with a question back to me** that checks whether I actually understood, ideally one
  that requires applying it to LilOS rather than reciting a definition.

Remember I'm shaky on pointers vs references, ownership, lifetimes, RAII, const-correctness,
and UB — so don't assume those as background knowledge when explaining something else.
