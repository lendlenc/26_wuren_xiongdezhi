from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score, classification_report

# 加载 sklearn 内置的鸢尾花数据集
iris = datasets.load_iris()
X = iris.data    # 4个特征: 花萼长度、花萼宽度、花瓣长度、花瓣宽度
y = iris.target  # 3个类别: Setosa, Versicolour, Virginica

# 划分训练集和测试集 (80% 训练，20% 测试，random_state保证每次结果一致)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 实例化 SVM
svm_model = SVC(kernel='linear', C=1.0)

# 训练模型
svm_model.fit(X_train, y_train)

# 对测试集进行预测
y_pred = svm_model.predict(X_test)
print(f"准确率: {accuracy_score(y_test, y_pred) * 100:.2f}%")
print(classification_report(y_test, y_pred, target_names=iris.target_names))
