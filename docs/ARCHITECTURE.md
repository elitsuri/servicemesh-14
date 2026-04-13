# Architecture

## Overview

servicemesh is structured as a production-oriented project with separated responsibilities across application code, infrastructure, and documentation.
The repository is intended to remain maintainable as the feature set expands and as additional contributors join the project.

## Top-Level Modules

- `.github/`: CI workflows, security policy, and contribution templates
- `database/`: schema, migrations, and seed data
- `src/`: application source code and core business modules

## Runtime Boundaries

- Keep request entry points thin and move business rules into dedicated services.
- Centralize persistence access in repositories or data-access modules.
- Keep configuration, security, and environment handling isolated from business code.
- Treat CI, deployment, and operational runbooks as first-class repository assets.

## Change Strategy

- Add new features by extending the existing module boundaries instead of creating ad-hoc utility files.
- Prefer incremental vertical slices: API, data model, tests, UI, and docs together.
- Update README and operations documentation whenever runtime behavior changes.
