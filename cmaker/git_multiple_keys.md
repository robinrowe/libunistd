

Open Git Bash.

Paste the text below, substituting in your GitHub email address.

$ ssh-keygen -t rsa -b 4096 -C "your_email@example.com"

Once you’re done, you will have two sets of SSH keys, e.g.:

    ~/.ssh/personal_key and ~/.ssh/personal_key.pub
    ~/.ssh/work_key and ~/.ssh/work_key.pub
	
Now create the file ~/.ssh/config and add the following contents:

Host gitlab.com
    HostName gitlab.com
    User git
    IdentityFile ~/.ssh/personal_keyHost gitlab.com-work
HostName gitlab.com
    User git
    IdentityFile ~/.ssh/work_key
	
$ git remote set-url origin git@gitlab.com-work:work/repository.git

Remove any existing credentials from Control Panel\User Accounts\Credential Manager

ssh -T git@gitlab.com:robinrowe

Working with non-default SSH key pair paths

If you used a non-default file path for your GitLab SSH key pair, you must configure your SSH client to find your GitLab private SSH key for connections to GitLab.

Open a terminal and use the following commands (replacing other_id_rsa with your private SSH key):

eval $(ssh-agent -s)
ssh-add ~/.ssh/other_id_rsa

Public SSH keys need to be unique to GitLab, as they will bind to your account. Your SSH key is the only identifier you’ll have when pushing code via SSH, that’s why it needs to uniquely map to a single user.
Per-repository SSH keys

If you want to use different keys depending on the repository you are working on, you can issue the following command while inside your repository:

git config core.sshCommand "ssh -o IdentitiesOnly=yes -i ~/.ssh/private-key-filename-for-this-repository -F /dev/null"


https://docs.gitlab.com/ee/ssh/