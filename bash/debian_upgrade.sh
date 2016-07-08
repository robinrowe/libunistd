cp /etc/apt/sources.list{,.bak}
sed -i -e 's/ \(stable\|wily\)/ xenial/ig' /etc/apt/sources.list
apt-get update
apt-get --download-only dist-upgrade
apt-get dist-upgrade
