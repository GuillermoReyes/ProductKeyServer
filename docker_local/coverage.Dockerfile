FROM gkhd_web:base as builder

COPY . /usr/src/projects/
WORKDIR /usr/src/projects/build

RUN pwd
RUN cmake -DCMAKE_BUILD_TYPE=Coverage ..
RUN make coverage