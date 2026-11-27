# Our textbook on machine learning and artificial intelligence is out

Robert Godin and I have a new book. The title is *Introduction to Machine Learning and Artificial Intelligence with Python*. The paperback came out today. It is 741 pages. The Kindle edition delivers on November 27.

The [paperback](https://www.amazon.com/dp/B0HT2JWXQ5/?tag=daniellemires-20) is on Amazon. The [Kindle edition](https://www.amazon.com/dp/B0GX32CJ74?tag=daniellemires-20) is there too.

Robert is a retired professor of computer science at the Université du Québec à Montréal. My own work is on software performance. We wrote the book as a first course, in a college program or at university, and for a programmer who already writes Python and wants to see how these systems work.

Basic Python is enough. Some probability, statistics or calculus makes a few chapters easier, and when one of those ideas appears we give you the intuition to keep going. The emphasis is on using the methods, and on implementing them.

You begin with NumPy, including broadcasting and `einsum`, and with Pandas. The tables are real: the California housing set, and stock prices loaded through yfinance. Linear regression comes next, from the normal equations and from gradient descent, and then as the same fit in scikit-learn. Multivariate regression adds scaling, polynomial features, a held-out test set, cross-validation, ridge, lasso and elastic net. Classification covers logistic regression, a Bayesian classifier and decision trees. Time series runs from an autoregressive model through ARIMA, on real series, with the autocorrelation plots you use to decide that a model is done.

We then build a neural network ourselves. A dense layer, the forward pass, backpropagation, mini-batches, a nonlinearity, softmax with cross-entropy, and Xavier and Kaiming initialization. Correlation and convolution are worked out with padding and stride before they become a layer. PyTorch comes after that, once you have seen where the gradients come from. That chapter trains on MNIST and Fashion-MNIST, then LeNet-5 and AlexNet on a GPU, with pooling, dropout, batch normalization, skip connections, a learning-rate schedule, fine-tuning and data augmentation.

Sequences open with a recurrent network and a character-level language model, then a word-level model. The transformer is introduced as sequence modeling without the recurrence. From there the book uses the model libraries: sentiment analysis, text generation, translation, the BERT family, sentence embeddings, zero-shot classification, and fine-tuning with LoRA.

A later chapter is about how a current language model differs from the 2017 transformer. You implement byte-pair encoding on a small corpus and watch common fragments emerge. Then come the key-value cache, grouped-query attention, sliding-window and sparse attention, rotary positions (RoPE), mixture of experts, and quantization, down through NF4 and QLoRA.

Reinforcement learning starts from the return of an episode and the Bellman equation. It continues through policy iteration, Monte Carlo, temporal-difference learning and policy gradients, and then to the training behind chat models: supervised fine-tuning, a reward model, reinforcement learning from human feedback, and direct preference optimization.

The last chapter is about shipping a model. Docker, so the same Python, PyTorch and CUDA versions come up on another machine. The exercise is a sentiment-analysis service in a container. The chapter also shows what `torch.compile` changes for GPU inference.

Each chapter has runnable examples and exercises, and the exercises have solutions.

The paperback is [on Amazon](https://www.amazon.com/dp/B0HT2JWXQ5/?tag=daniellemires-20). The Kindle edition is [here](https://www.amazon.com/dp/B0GX32CJ74?tag=daniellemires-20).
