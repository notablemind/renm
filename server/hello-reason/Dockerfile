FROM ocaml/opam:alpine as base

RUN sudo apk update
RUN sudo apk add m4
RUN sh -c "cd ~/opam-repository && git pull --quiet"
RUN opam update
RUN opam install dune reason

# need these two for building tls, which is needed by cohttp
RUN opam depext conf-gmp.1
RUN opam depext conf-perl.1
RUN opam install tls
# for cohttp stuffs
RUN opam install lwt cohttp cohttp-lwt-unix

# ok build our thing
COPY --chown=opam:nogroup . /home/opam/hello-reason
WORKDIR /home/opam/hello-reason
RUN sh ./build.sh

FROM alpine
# this is needed by the tls impl
RUN apk add gmp-dev

COPY --from=base /home/opam/hello-reason/_build/default/bin/Server.exe /server
CMD ["/server"]
