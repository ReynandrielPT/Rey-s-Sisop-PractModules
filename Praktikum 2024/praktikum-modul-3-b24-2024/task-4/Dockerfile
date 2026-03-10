FROM php:8.2-fpm

RUN apt-get update
RUN apt-get install -y wget
RUN apt-get install -y zip
RUN apt-get install -y unzip
RUN apt-get install -y libpng-dev
RUN apt-get install -y libonig-dev
RUN apt-get install -y libxml2-dev
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
RUN docker-php-ext-install pdo pdo_mysql mbstring exif pcntl bcmath gd

WORKDIR /var/www/html
COPY . .
RUN wget --no-check-certificate "https://drive.usercontent.google.com/u/0/uc?id=1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK&export=download" -O laraveltemplate
RUN unzip laraveltemplate
RUN mv laravel-onion-template/* /var/www/html
RUN rm laraveltemplate
RUN rm -rf laravel-onion-template

RUN echo "APP_NAME=Laravel" > .env
RUN echo "APP_ENV=local" >> .env
RUN echo "APP_KEY=base64:IkpV7upEVtsaFPo7yUeE7M2cknBww9N8vglFG5J2NdU=" >> .env
RUN echo "APP_DEBUG=true" >> .env
RUN echo "APP_URL=http://localhost" >> .env
RUN echo "LOG_CHANNEL=stack" >> .env
RUN echo "LOG_DEPRECATIONS_CHANNEL=null" >> .env
RUN echo "LOG_LEVEL=debug" >> .env
RUN echo "DB_CONNECTION=mysql" >> .env
RUN echo "DB_HOST=mysql" >> .env
RUN echo "DB_PORT=3306" >> .env
RUN echo "DB_DATABASE=keyisa" >> .env
RUN echo "DB_USERNAME=root" >> .env
RUN echo "DB_PASSWORD=secret" >> .env
RUN echo "BROADCAST_DRIVER=log" >> .env
RUN echo "CACHE_DRIVER=file" >> .env
RUN echo "FILESYSTEM_DISK=local" >> .env
RUN echo "QUEUE_CONNECTION=sync" >> .env
RUN echo "SESSION_DRIVER=file" >> .env
RUN echo "SESSION_LIFETIME=120" >> .env
RUN echo "MEMCACHED_HOST=127.0.0.1" >> .env
RUN echo "REDIS_HOST=127.0.0.1" >> .env
RUN echo "REDIS_PASSWORD=null" >> .env
RUN echo "REDIS_PORT=6379" >> .env
RUN echo "MAIL_MAILER=smtp" >> .env
RUN echo "MAIL_HOST=smtp.mailtrap.io" >> .env
RUN echo "MAIL_PORT=2525" >> .env
RUN echo "MAIL_USERNAME=7d009e488ee2ca" >> .env
RUN echo "MAIL_PASSWORD=bb9becb07b369a" >> .env
RUN echo "MAIL_ENCRYPTION=tls" >> .env
RUN echo "AWS_ACCESS_KEY_ID=" >> .env
RUN echo "AWS_SECRET_ACCESS_KEY=" >> .env
RUN echo "AWS_DEFAULT_REGION=us-east-1" >> .env
RUN echo "AWS_BUCKET=" >> .env
RUN echo "AWS_USE_PATH_STYLE_ENDPOINT=false" >> .env
RUN echo "PUSHER_APP_ID=" >> .env
RUN echo "PUSHER_APP_KEY=" >> .env
RUN echo "PUSHER_APP_SECRET=" >> .env
RUN echo "PUSHER_APP_CLUSTER=mt1" >> .env
RUN echo "MIX_PUSHER_APP_KEY="\${PUSHER_APP_KEY}"" >> .env
RUN echo "MIX_PUSHER_APP_CLUSTER="\${PUSHER_APP_CLUSTER}"" >> .env

COPY --from=composer:latest /usr/bin/composer /usr/bin/composer
RUN composer install
COPY --chown=www-data:www-data . /var/www/html
EXPOSE 80