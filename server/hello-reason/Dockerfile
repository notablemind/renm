FROM jaredly/opam:cohttp as base

# ok build our thing
COPY --chown=opam:nogroup . /hello-reason
WORKDIR /hello-reason
RUN sh -c 'eval `opam config env` dune build bin/Server.exe'

# FROM alpine
# this is needed by the tls impl
# RUN apk add gmp-dev
FROM scratch

COPY --from=base /hello-reason/_build/default/bin/Server.exe /server
ENTRYPOINT ["/server"]
