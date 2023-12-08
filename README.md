# Provider Authors: Writing a Skeleton Provider - a webinar

TODO.md enumerates the topics to be presented.
slides.pdf are the slides from the webinar

Everything else is code, divided into different tags and branches to cover
each topic separately.  Each one of them has a Makefile with two targets,
'all' (default) to build, and 'try' to try it out.  `make && make try` is
should be expected to build and show you the result.

-   *bare* (tag)

    Slides title: "Skeleton: A minimum nonsense provider"

-   *built_in* (branch)

    Slides title: "Skeleton: A built-in nonsense provider"

-   *prov_params* (branch)

    Slides title: "Skeleton: Handling parameter requests"

-   *operation* (branch)

    Slides title: "Skeleton: Query function"

-   *core_params* (branch)

    Slides title: "Skeleton: Getting config data"

-   *provctx* (branch)

    Slides title: "Skeleton: Provider context"

-   *errors* (branch)

    Slides title: "Skeleton: Provider context"

-   *libctx-selfcontained* (branch)

    Slides titles:
    -   "Skeleton: Library context"
    -   "Skeleton: Library context - self-contained model"

-   *libctx-inheriting* (branch)

    Slides title:
    -   "Skeleton: Library context"
    -   "Skeleton: Library context - inheriting model"
