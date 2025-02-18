FROM node:23-alpine

WORKDIR /app

COPY /app/package.json ./
RUN ls -la /app
RUN npm install
RUN ls -la /app/node_modules/express
CMD ["node", "app.js"]
