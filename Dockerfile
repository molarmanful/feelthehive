FROM node:19
WORKDIR /usr/src/app
COPY package*.json ./
RUN npm ci
COPY . .
ENV PORT 8080
EXPOSE 8000
CMD ["npm", "start"]