# Style Prompts

Reusable style/voice prompts derived from the writing at
[sheharyaar.in/blog](https://www.sheharyaar.in/blog). Drop the relevant file
into an agent (system prompt, project instructions, or as context) so its
output sounds like me instead of generic LLM prose.

## When to use which

| File | Use it for |
|---|---|
| [`technical-blog.md`](./technical-blog.md) | Exploratory posts that chase a question I had and report what I found. Narrative + curiosity carry it. Not a manual. |
| [`technical-tutorial.md`](./technical-tutorial.md) | Step-by-step guides where the reader must successfully *do the thing* by the end. Contract is correctness and reproducibility. |
| [`codebase-agent-interaction.md`](./codebase-agent-interaction.md) | How an agent should communicate while working with me in a repo: chat, plans, clarifying questions, commits, PRs, code comments. |

## Shared spine

All three enforce the same four non-negotiable voice traits and the same
banned-LLM-tic list:

1. **Personal motivation** — first person, curious-learner stance, why *I*
   cared. Never open on the topic's general importance.
2. **Scope with a complexity choice** — offer the user a depth level (and ask
   if unchosen); stay within it, no rabbit holes.
3. **Rhetorical questions as real scaffolding** — pose the actual question,
   then answer it.
4. **Personality & informality** — friendly, a little whimsical, never
   corporate-neutral.

They differ in what they let the agent *improve* past my own habits:
the tutorial pushes hardest on structure (numbered steps, expected output,
reproducibility); the codebase/agent prompt adds a dedicated **honesty**
section (admit limits, report failures faithfully, don't claim done until
verified).

## How these were made

Built by analyzing the blog's voice against default LLM output, then refined
to keep the traits I wanted and free the agent to raise quality on
definitions, structure, and mechanism narration (I'm not a professional
editor).
