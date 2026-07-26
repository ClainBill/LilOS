---
description: Quiz me on my own code to check I actually understand it
argument-hint: [file or topic, or blank for recent changes]
---

Quiz me on: ${ARGUMENTS:-the code I've changed recently (check git log and git diff)}

Ask me **one question at a time** and wait for my answer. Don't provide the answer in the
question, and don't move on until I've responded.

Good questions to draw from:

- "What happens if this is called with `x = -1`?"
- "Why is this parameter a reference and not a pointer?"
- "Who owns this memory, and when is it freed?"
- "What would break if I changed this `const`?"
- "Trace this loop for me with `r = 1`."
- "Why did we choose this over the alternative?"

Prioritise memory, ownership, lifetimes, and UB — that's my weak spot.

When I answer:

- If I'm **right**, say so briefly and go deeper rather than moving on to something unrelated.
- If I'm **partly right**, tell me which part and let me have another go before correcting me.
- If I'm **wrong**, don't just correct me — show me the concrete case that disproves my answer,
  ideally by pointing at real code or a value that breaks it.
- If I say **"I don't know"**, that's a fine answer. Explain it properly, then ask a variation
  later to check it landed.

Five or six questions is plenty. At the end, tell me honestly which areas looked solid and
which need work — I'd rather know than be flattered.
