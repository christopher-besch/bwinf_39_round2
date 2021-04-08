const path = require("path");

module.exports = {
    entry: "./src/index.ts",
    module: {
        rules: [
            {
                test: /\.ts$/,
                include: [path.resolve(__dirname, "src")],
                use: "ts-loader",
            },
        ],
    },
    output: {
        publicPath: "public",
        path: path.resolve(__dirname, "public"),
    },
    devServer: {
        publicPath: "./",
        contentBase: "./public",
        hot: true,
    },
};
