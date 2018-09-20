FROM ocaml/opam:alpine as base

RUN sudo apk update
RUN sudo apk add m4
RUN sh -c "cd ~/opam-repository && git pull -q"
RUN opam update
RUN opam install dune reason > /dev/null 2>&1

# need these two for building tls, which is needed by cohttp
RUN opam depext conf-gmp.1
RUN opam depext conf-perl.1
RUN opam install tls > /dev/null 2>&1
# for cohttp stuffs
RUN opam install lwt cohttp cohttp-lwt-unix > /dev/null 2>&1

# ok build our thing
COPY --chown=opam:nogroup . /hello-reason
WORKDIR /hello-reason
RUN sh -c 'eval `opam config env` dune build bin/Server.exe'

FROM alpine
# this is needed by the tls impl
RUN apk add gmp-dev

COPY --from=base /hello-reason/_build/default/bin/Server.exe /server
CMD ["/server"]
