# OpenCode Agent

Overview
- This file documents the OpenCode agent that assists with software engineering tasks in this workspace.
- The agent interacts with project files, issues, and automation tools to perform non-destructive edits, scaffolding, and basic repo maintenance.

Purpose
- Accelerate code and docs changes by proposing, applying, and validating changes in a controlled way.
- Provide a consistent, auditable trail of edits via patches and change summaries.

Capabilities
- Read project files and summarize content.
- Create and apply patches (via patch format) to modify files.
- Add and update documentation files (README, docs/, agent.md, etc.).
- Create and modify lightweight scaffolds (docs scaffolding, skeleton code) without touching production logic.
- Propose and draft commit messages and pull request summaries (without pushing to remotes unless explicitly requested).
- Run or simulate basic repository operations using provided tooling (read, glob, grep, patch, etc.).

Interaction model
- Work is performed via a sequence of tool invocations (Read, Write/Edit, Apply Patch, Glob, etc.).
- When a user asks for changes, the agent proposes edits and applies patches when approved.
- For complex tasks, the agent can break work into a plan and present it as a todo list or patch set.

Safety and governance
- Avoid destructive actions like pushes or force-pushes unless explicitly requested.
- Do not reveal secrets or modify credentials. Non-secret changes only.
- Maintain ASCII text unless there is a justified reason to include non-ASCII characters.

Common workflows
- Documentation scaffolding: create docs/README.md, API, Design, Guides skeletons.
- Code scaffolding: add minimal header/source stubs in include/src and a tiny example.
- Readme updates: improve clarity, add references to design documents (e.g., docs/cpp_aquacrop_design.md).
- Patch-based edits: generate patches in the patch format and apply them through the patch tool.

Example task
- Task: Add a new design document for the C++ port and reference it from the README.
- Steps:
  1) Create docs/cpp_aquacrop_design.md with design content.
  2) Update README.md to link to the new design document.
  3) Commit or present a patch for review (no remote push unless asked).

Note: This document describes the agent's intended usage in this workspace. If you need a concrete action (e.g., create a file, patch, or run a test), describe it and I will perform it.
