FROM ocaml/opam:alpine as base

USER root
RUN apk add m4
USER opam
RUN opam update
RUN sh -c "cd ~/opam-repository && git pull"
RUN opam update
RUN opam install dune reason
# for the hello-reason example
RUN opam install lwt cohttp cohttp-lwt-unix
RUN opam depext conf-gmp.1
RUN sudo apk update
RUN opam depext conf-perl.1
RUN opam install tls
COPY --chown=opam:nogroup . /home/opam/hello-reason
WORKDIR /home/opam/
WORKDIR /home/opam/hello-reason
RUN chmod +x ./build.sh
RUN ./build.sh

# USER root
# RUN cp /home/opam/hello-reason/_build/default/bin/Hello.exe /hello
# USER opam

# FROM ocaml/opam:alpine
# USER root
# RUN apk add m4
# USER opam

FROM alpine
RUN apk add m4
RUN apk add gmp-dev

COPY --from=base /home/opam/hello-reason/_build/default/bin/Hello.exe /hello
COPY --from=base /home/opam/hello-reason/_build/default/bin/Server.exe /reddit
CMD ["OCAMLRUNPARAM=b /hello"]
