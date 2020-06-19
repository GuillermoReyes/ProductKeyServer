### Build/test container ###
#local dockerfile for local debugging use
# Define builder stage
FROM gkhd_web:base as builder

# Share work directory

COPY . /usr/src/projects/
WORKDIR /usr/src/projects/build

#RUN apt-get update && apt-get install -y cmake libgtest-dev libboost-test-dev && rm -rf /var/lib/apt/lists/* 

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure
RUN find . -name "webserver"


### Deploy container ###
# Define deploy stage
FROM ubuntu:bionic as deploy

RUN apt-get update && apt-get install -y curl

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/projects/build/bin/webserver .
COPY --from=builder /usr/src/projects/tests/staticConfigLocal .
COPY --from=builder /usr/src/projects/data/* .
COPY --from=builder /usr/src/projects/tests/my_config .


EXPOSE 80
EXPOSE 8001

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
ENTRYPOINT ["./webserver"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
CMD ["my_config"]
