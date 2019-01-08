rm -f prob1.zip
zip -r prob1.zip . -x "*.git*" "*cs231n/datasets*" "*.ipynb_checkpoints*" "*README.md" "*collect_submission.sh" "*requirements.txt" ".env/*" "*.pyc" "*cs231n/build/*" "__pycache__/*"
