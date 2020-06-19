### Build/test container ###
# Define builder stage
FROM gkhd_web:base as builder

# Share work directory
COPY . /usr/src/projects
WORKDIR /usr/src/projects/build

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
# Define deploy stage
FROM ubuntu:bionic as deploy

RUN apt-get update && apt-get install -y curl

COPY --from=builder /usr/src/projects/build/bin/webserver .
COPY --from=builder /usr/src/projects/tests/staticConfigLocal .

COPY --from=builder /usr/src/projects/tests/my_config .

RUN mkdir -p ./html

COPY --from=builder /usr/src/projects/html ./html
COPY --from=builder /usr/src/projects/content ./content


EXPOSE 80
EXPOSE 8001

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
ENTRYPOINT ["./webserver"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
CMD ["my_config"]
