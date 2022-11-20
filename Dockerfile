FROM node:19
WORKDIR /usr/src/app
COPY . .
RUN npm ci && node ./ext/compile.js
ENV PORT 8080
EXPOSE 8000
CMD ["npm", "start"]