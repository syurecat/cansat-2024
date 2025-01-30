FROM node:3.8-slim-buster

ARG dir=/app

WORKDIR $dir

COPY package*.json ./

RUN npm install

CMD ["node", "main.js"]
