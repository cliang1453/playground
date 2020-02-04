# Hidden Markov Model Prediction on Genes

## General

Remap infrequent words to _RARE_ class
```
python3 remap_infrequent_words.py gene.train > gene.remapped_train
```

Count remapped wordtag and n-grams
```
python3 count_freqs.py gene.remapped_train > gene.remapped_counts
```

## Baseline

Training and prediction
```
python3 run.py gene.remapped_counts gene.test 0 > gene_test.p1.out
```

Evaluation
```
python3 eval_gene_tagger.py gene.key gene_test.p1.out
```

## Using Tri-gram Features

Training and prediction
```
python3 run.py gene.remapped_counts gene.test 1 > gene_test.p2.out
```

Evaluation
```
python3 eval_gene_tagger.py gene.key gene_test.p2.out
```